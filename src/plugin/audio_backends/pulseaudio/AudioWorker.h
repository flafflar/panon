#pragma once

#include <QObject>

#include <cstdint>
#include <limits>
#include <memory>
#include <ranges>
#include <vector>

#include <pulse/context.h>
#include <pulse/def.h>
#include <pulse/introspect.h>
#include <pulse/mainloop.h>
#include <pulse/proplist.h>
#include <pulse/stream.h>

// TODO: Make this header private somehow?
#include "PulseAudioDeviceInfo.h"

static_assert(
    std::numeric_limits<float>::is_iec559,
    "The float type must be of the IEEE 754 standard for this library to work");
static_assert(sizeof(float) * 8 == 32,
              "The float type must have a width of exactly 32 bits");

class AudioWorker : public QObject {
  Q_OBJECT

public:
  AudioWorker();

  // Connects to the Pulse server, and starts running in a loop, processing
  // incoming data.
  void run();

  std::ranges::ref_view<std::vector<std::shared_ptr<PulseAudioDeviceInfo>>>
  devices() {
    return this->m_devices;
  }

  // TODO: Performance? Does this copy the vector?
  std::vector<float> getLeftBuffer() const { return this->bufferLeft; }
  std::vector<float> getRightBuffer() const { return this->bufferRight; }

  void setCurrentDevice(std::shared_ptr<PulseAudioDeviceInfo> device);

  void setFps(uint32_t fps);

Q_SIGNALS:
  // Indicates that the device list has changed (e.g. a device got added or
  // removed).
  void devicesChanged();

  // Indicates that we have new audio data into the buffers.
  void buffersUpdated();

  // Indicates that an error has occured.
  void error(QString &errorMessage);

private:
  pa_mainloop *loop;
  pa_proplist *proplist;
  pa_context *context;

  // The stream that receives audio from the selected device.
  pa_stream *stream = nullptr;

  // Update the rate at which the stream receives data, based on the current fps
  // value.
  void updateStreamFps();

  // The buffer for the left audio channel.
  std::vector<float> bufferLeft;
  // The buffer for the right audio channel.
  std::vector<float> bufferRight;

  // The callback invoked from libpulse when the context changes state.
  static void contextStateCallback(pa_context *context, void *data);

  // The callback called when we receive a new event from the server.
  static void subscribedEventCallback(pa_context *context,
                                      pa_subscription_event_type_t t,
                                      uint32_t idx, void *data);

  static void streamCallback(pa_stream *stream, size_t nbytes, void *data);

  QString m_errorMessage;

  std::vector<std::shared_ptr<PulseAudioDeviceInfo>> m_devices;

  std::shared_ptr<PulseAudioDeviceInfo> m_currentDevice;

  uint32_t m_fps;

  // A flag indicating the sinkInfoCallback is done and has added all sinks in
  // the device list.
  bool sinksDone;
  // A flag indicating the sourceInfoCallback is done and has added all sources
  // in the device list.
  bool sourcesDone;

  // Gets a list of all the audio devices from the server and stores them in
  // m_devices.
  void scanDevices();

  // Invoked when the connection to the Pulse server is established.
  void onReady();

  void onCurrentDeviceChange();

  // Called when the stream is ready in order to initiate receiving audio data
  // from the server.
  void startRecording();
};
