#pragma once

#include <complex>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include <fftw3.h>

// Make sure fftwf_complex and std::complex are binary-compatible.
static_assert(sizeof(std::complex<float>) == sizeof(fftwf_complex));

/**
 * A utility struct that contains references to the input buffers of an
 * AudioBufferProcessor, plus a mutex guard that locks the buffers while the
 * struct is in scope.
 *
 * @see AudioBufferProcessor
 */
struct AudioBufferProcessorInputs {
  std::vector<float> &left;
  std::vector<float> &right;

  const std::lock_guard<std::mutex> guard;
};

/**
 * A utility struct that contains references to the output buffers of an
 * AudioBufferProcessor, plus a mutex guard that locks the buffers while the
 * struct is in scope.
 *
 * @see AudioBufferProcessor
 */
struct AudioBufferProcessorOutputs {
  const std::vector<float> &left;
  const std::vector<float> &right;
  const std::vector<float> &mono;
  const std::vector<std::complex<float>> &fft;

  const std::lock_guard<std::mutex> guard;
};

/**
 * A processor class that receives audio data, processes them in another thread,
 * and returns the results.
 */
class AudioBufferProcessor {
public:
  AudioBufferProcessor();

  /**
   * Connect this processor to some input buffers.
   */
  void setInputs(std::vector<float> &left, std::vector<float> &right,
                 std::mutex &mutex);

  /**
   * Connect this processor to some output buffers.
   */
  void setOutputs(std::mutex &mutex, std::vector<float> &left,
                  std::vector<float> &right, std::vector<float> &mono);

  /**
   * Get a struct that contains references to the inpus buffers so you can write
   * in them, plus a mutex guard that gives you unique access to the buffers.
   *
   * Caution: the vectors provided might not be empty, but any data they contain
   * are invalid. Make sure to resize them as needed and overwrite them whole.
   */
  [[gnu::always_inline]]
  AudioBufferProcessorInputs getInputs() {
    return {.left = this->inLeft,
            .right = this->inRight,
            .guard = std::lock_guard(this->inMutex)};
  }

  /**
   * Signal to the processor that the input buffers have changed, which means
   * that it will have to recalculate everything.
   *
   * This function is very cheap to call, because the recalculation will happen
   * on another thread.
   */
  void signalDirty();

  /**
   * Get a struct that contains references to the output buffers so you can read
   * from them, plus a mutex guard that gives you unique access to the buffers.
   */
  [[gnu::always_inline]]
  AudioBufferProcessorOutputs getOutputs() {
    return {.left = this->outLeft,
            .right = this->outRight,
            .mono = this->outMono,
            .fft = this->outFFT,
            .guard = std::lock_guard(this->outMutex)};
  }

  /**
   * Sets the callback that will be called every time the output buffers are
   * updated.
   *
   * Beware that the callback will be called in a different thread, so make sure
   * whatever you do in it is thread-safe.
   */
  void onUpdate(std::function<void()> callback);

private:
  /** The buffer of the left channel we receive as input. */
  std::vector<float> inLeft;
  /** The buffer of the right channel we receive as input. */
  std::vector<float> inRight;

  /** The mutex controlling access to inBufferLeft and inBufferRight. */
  std::mutex inMutex;

  /** The buffer that we will output the left channel to. */
  std::vector<float> outLeft;
  /** The buffer that we will output the right channel to. */
  std::vector<float> outRight;
  /** The buffer that we will output the mono channel to. */
  std::vector<float> outMono;
  /** The buffer that we will output the FFT transform of the audio to. */
  std::vector<std::complex<float>> outFFT;

  /** The mutex controlling access to all the output buffers. */
  std::mutex outMutex;

  /**
   * Whether the input buffers have changed since the last time we processed
   * them, and we need to reprocess them.
   */
  bool dirty = false;
  /** The mutex controlling access to the dirty variable. */
  std::mutex dirtyMutex;
  /**
   * The condition variable that is used to signal to the processing thread that
   * it needs to rerun because the input buffers have changed.
   */
  std::condition_variable dirtyCondVar;

  std::vector<float> scratchLeft;
  std::vector<float> scratchRight;
  /** An intermediate buffer that we write the mono channel data in. */
  std::vector<float> scratchMono;
  /** An intermediate buffer that holds the result of the FFT. */
  std::vector<std::complex<float>> scratchFFT;

  /** The thread that runs the processing on the buffers. */
  std::jthread thread;

  /** The main function that runs on the processing thread. */
  void threadFn();

  /**
   * The main function of this class. Reads the input data, processes them, and
   * writes the results in the outputs.
   */
  void process();

  /** The callback that will be called every time the outputs are updated. */
  std::function<void()> callback;
};
