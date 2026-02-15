#include "AudioBufferProcessor.h"

#include <mutex>
#include <thread>

#include <QDebug>

#include <fftw3.h>

/**
 * Creating plans in the fftw library is not thread safe, so we use this mutex
 * to make sure only one thread is creating a plan at a time.
 */
std::mutex fftwPlanMutex;

AudioBufferProcessor::AudioBufferProcessor() {
  // Start the thread immediately.
  // TODO: Mechanism to stop the thread.
  this->thread = std::jthread(&AudioBufferProcessor::threadFn, this);
}

void AudioBufferProcessor::signalDirty() {
  // We create a separate scope in order to lock the dirtyMutex only for the
  // duration of the block.
  {
    std::lock_guard guard(this->dirtyMutex);

    if (this->dirty) {
      qWarning() << "A buffer was dropped before being processed";
    }

    this->dirty = true;
  };

  // Notify the processing thread that the dirty flag has changed.
  this->dirtyCondVar.notify_one();
}

void AudioBufferProcessor::onUpdate(std::function<void()> callback) {
  this->callback = callback;
}

void AudioBufferProcessor::threadFn() {
  while (true) {
    // We create a separate scope in order to lock the dirtyMutex only for the
    // duration of the block.
    {
      std::unique_lock lock(this->dirtyMutex);
      // Wait until we get a signal that dirty is true.
      this->dirtyCondVar.wait(lock, [this] { return this->dirty; });
      // Reset the dirty flag to false while we have the lock.
      this->dirty = false;
    }

    this->process();
  }
}

void AudioBufferProcessor::process() {
  // We create a separate scopre in order to lock the inputs only for the
  // duration of the block.
  {
    std::lock_guard guard(this->inMutex);

    // Copy the input buffers to the scratch buffers.
    // Since the data in the input buffers will be overwritten before the next
    // call, it's faster to swap them instead of copying them.
    this->scratchLeft.swap(this->inLeft);
    this->scratchRight.swap(this->inRight);
  };

  // How many samples are contained in the buffers. Both the left and right
  // buffers should contain the same amount of samples, so we arbitrarily get
  // the length of the left buffer.
  size_t samples = this->scratchLeft.size();

  // Prepare the mono buffer for writing new data in it.
  this->scratchMono.resize(this->scratchLeft.size());

  for (size_t i = 0; i < samples; i++) {
    this->scratchMono[i] = (this->scratchLeft[i] + this->scratchRight[i]) / 2.0;
  }

  // The FFT can only be performed on arrays with an even count of samples, so
  // we round our samples count down to the nearest multiple of 2 (so we will
  // essentially ignore the last sample if the sample count is even).
  int fftSize = samples - (samples % 2);

  // Prepare the FFT buffer.
  // Since our input data is real, the FFT is symmetric around the center, which
  // means we only need half the number of samples for the FFT result.
  this->scratchFFT.resize(fftSize / 2 + 1);

  // Create a plan for running the FFT.
  // This call can be very expensive, because FFTW runs multiple implementations
  // of the FFT and times them, to see which one is faster in this specific
  // machine. (FFTW claims that this can take several seconds, but on my machine
  // it took 60ms for a 1470 sample array). But, after making this timing once,
  // it caches the results, so in subsequent calls with the same array size it
  // uses the algorithm from the cache. This means that this will have a
  // performance impact only when the buffer size changes. Also, since this runs
  // in a different thread, it will never block the application in the case it
  // is taking too long to compute.
  // TODO: Performance-wise this is not the best, but this is easier to
  // implement, because a plan is made to operate on the same arrays repeatedly,
  // but our arrays are constantly swapped back and forth, which means their
  // addresses change. After I have this working correctly I can maybe implement
  // a better version where I don't recompute the plan every time.
  fftwf_plan fftPlan;
  {
    std::lock_guard guard(fftwPlanMutex);

    fftPlan = fftwf_plan_dft_r2c_1d(
        this->scratchMono.size(), this->scratchMono.data(),
        reinterpret_cast<fftwf_complex *>(this->scratchFFT.data()),
        FFTW_MEASURE);
  };

  fftwf_execute(fftPlan);

  fftwf_destroy_plan(fftPlan);

  // Again, we create a separate scope in order to lock the outputs for the
  // duration of the block.
  {
    std::lock_guard guard(this->outMutex);

    // Copy the data from the scratch buffers to the outputs.
    // Since we are going to rewrite the scratch buffers on the next iteration,
    // it's faster to swap them with the old output buffers instead of copying.
    this->outLeft.swap(this->scratchLeft);
    this->outRight.swap(this->scratchRight);
    this->outMono.swap(this->scratchMono);
    this->outFFT.swap(this->scratchFFT);
  };
}
