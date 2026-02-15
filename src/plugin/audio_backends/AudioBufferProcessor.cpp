#include "AudioBufferProcessor.h"

#include <QDebug>
#include <mutex>
#include <thread>

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
  };
}
