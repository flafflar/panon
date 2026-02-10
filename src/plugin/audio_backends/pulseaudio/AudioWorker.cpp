#include "AudioWorker.h"

#include <cstdint>
#include <memory>

#include <QDebug>

#include <pulse/channelmap.h>
#include <pulse/context.h>
#include <pulse/def.h>
#include <pulse/error.h>
#include <pulse/introspect.h>
#include <pulse/operation.h>
#include <pulse/sample.h>
#include <pulse/stream.h>
#include <pulse/subscribe.h>
#include <qlogging.h>
#include <qtmetamacros.h>

#include "PulseAudioDeviceInfo.h"

AudioWorker::AudioWorker() : m_errorMessage(), m_currentDevice(nullptr) {
  // Initialize the properties of the Pulse client, which will be the same in
  // every connection.
  this->proplist = pa_proplist_new();
  pa_proplist_sets(this->proplist, PA_PROP_APPLICATION_NAME,
                   "xyz.flafflar.panon");

  this->loop = pa_mainloop_new();
  pa_mainloop_api *api = pa_mainloop_get_api(this->loop);

  this->context =
      pa_context_new_with_proplist(api, "xyz.flafflar.panon", this->proplist);

  pa_context_set_state_callback(this->context,
                                &AudioWorker::contextStateCallback, this);
}

void AudioWorker::setCurrentDevice(
    std::shared_ptr<PulseAudioDeviceInfo> device) {
  this->m_currentDevice = device;

  this->onCurrentDeviceChange();
}

void AudioWorker::contextStateCallback(pa_context *context, void *userdata) {
  AudioWorker *self = static_cast<AudioWorker *>(userdata);

  pa_context_state_t state = pa_context_get_state(context);

  switch (state) {
  case PA_CONTEXT_UNCONNECTED:
    qDebug() << "The context hasn't been connected yet";
    break;

  case PA_CONTEXT_CONNECTING:
    qDebug() << "A connection is being established";
    break;

  case PA_CONTEXT_AUTHORIZING:
    qDebug() << "The client is authorizing itself to the daemon";
    break;

  case PA_CONTEXT_SETTING_NAME:
    qDebug() << "The client is passing its application name to the daemon";
    break;

  case PA_CONTEXT_READY:
    qDebug() << "The connection is established";
    self->onReady();
    break;

  case PA_CONTEXT_FAILED:
    qDebug() << "The connection failed or was disconnected";
    break;

  case PA_CONTEXT_TERMINATED:
    qDebug() << "The connection was terminated cleanly";
    break;

  default:
    qDebug() << "Unknown context state " << state;
    break;
  }
}

void AudioWorker::subscribedEventCallback(pa_context *context,
                                          pa_subscription_event_type_t t,
                                          uint32_t idx, void *data) {
  AudioWorker *self = static_cast<AudioWorker *>(data);

  // Make sure we operate on the same context.
  Q_ASSERT(context == self->context);

  // The facility that caused the event (e.g. sink, source).
  int event_facility = PA_SUBSCRIPTION_EVENT_FACILITY_MASK & t;

  // The type of the event (one of NEW, CHANGE, REMOVE).
  int event_type = PA_SUBSCRIPTION_EVENT_TYPE_MASK & t;

  switch (event_facility) {
  case PA_SUBSCRIPTION_EVENT_SINK:

    switch (event_type) {
    case PA_SUBSCRIPTION_EVENT_NEW:
      // A new sink was added.
      // We need to add the new sink to the list.
      pa_context_get_sink_info_by_index(
          context, idx,
          [](pa_context *context, const pa_sink_info *info, int eol,
             void *data) {
            AudioWorker *self = static_cast<AudioWorker *>(data);

            // Make sure we operate on the same context.
            Q_ASSERT(context == self->context);

            // eol is true when we got the last message. In this case, we only
            // expect to get one message, so this means that this function will
            // be called twice: once with eol false, and once with eol true. I
            // could just ignore the second call when eol is true and notify
            // during the first call, but I'm not sure how libpulse behaves when
            // multiple sinks are queried at the same time, so I think it's
            // safer if I notify here, to keep the code the same as when
            // receiving multiple sinks.
            if (eol) {
              Q_EMIT self->devicesChanged();
              return;
            }

            auto devInfo = std::make_shared<PulseAudioDeviceInfo>(info);
            self->m_devices.push_back(devInfo);
          },
          self);
      break;

    case PA_SUBSCRIPTION_EVENT_CHANGE:
      // Some existing sink was changed.
      // We should retrieve the new information about the sink and update the
      // list entry.
      pa_context_get_sink_info_by_index(
          context, idx,
          [](pa_context *context, const pa_sink_info *info, int eol,
             void *data) {
            AudioWorker *self = static_cast<AudioWorker *>(data);

            // Make sure we operate on the same context.
            Q_ASSERT(context == self->context);

            // eol is true when we got the last message. In this case, we only
            // expect to get one message, so this means that this function will
            // be called twice: once with eol false, and once with eol true. I
            // could just ignore the second call when eol is true and notify
            // during the first call, but I'm not sure how libpulse behaves when
            // multiple sinks are queried at the same time, so I think it's
            // safer if I notify here, to keep the code the same as when
            // receiving multiple sinks.
            if (eol) {
              Q_EMIT self->devicesChanged();
              return;
            }

            auto devInfo = std::make_shared<PulseAudioDeviceInfo>(info);

            auto oldInfo =
                std::find_if(self->m_devices.begin(), self->m_devices.end(),
                             [info](auto elem) {
                               return elem.get()->pulseIndex() == info->index;
                             });

            if (oldInfo == self->m_devices.end()) {
              qDebug()
                  << "Received an update for a sink that is not in the list";

              // Since the sink does not exist in the list, just insert it.
              self->m_devices.push_back(devInfo);
            } else {
              // Replace the old element with the new device info.
              *oldInfo = devInfo;
            }
          },
          self);
      break;

    case PA_SUBSCRIPTION_EVENT_REMOVE:
      // A sink was removed.
      // We need to find the sink in the list and remove it.
      std::erase_if(self->m_devices, [idx](auto info) {
        return info.get()->pulseIndex() == idx;
      });

      Q_EMIT self->devicesChanged();

      break;
    }

    break;

  case PA_SUBSCRIPTION_EVENT_SOURCE:
    switch (event_type) {
    case PA_SUBSCRIPTION_EVENT_NEW:
      // A new source was added.
      // We need to add the new source to the list.
      pa_context_get_source_info_by_index(
          context, idx,
          [](pa_context *context, const pa_source_info *info, int eol,
             void *data) {
            AudioWorker *self = static_cast<AudioWorker *>(data);

            // Make sure we operate on the same context.
            Q_ASSERT(context == self->context);

            // eol is true when we got the last message. In this case, we only
            // expect to get one message, so this means that this function will
            // be called twice: once with eol false, and once with eol true. I
            // could just ignore the second call when eol is true and notify
            // during the first call, but I'm not sure how libpulse behaves when
            // multiple sources are queried at the same time, so I think it's
            // safer if I notify here, to keep the code the same as when
            // receiving multiple sources.
            if (eol) {
              Q_EMIT self->devicesChanged();
              return;
            }

            auto devInfo = std::make_shared<PulseAudioDeviceInfo>(info);
            self->m_devices.push_back(devInfo);
          },
          self);
      break;

    case PA_SUBSCRIPTION_EVENT_CHANGE:
      // Some existing source was changed.
      // We should retrieve the new information about the source and update the
      // list entry.
      pa_context_get_source_info_by_index(
          context, idx,
          [](pa_context *context, const pa_source_info *info, int eol,
             void *data) {
            AudioWorker *self = static_cast<AudioWorker *>(data);

            // Make sure we operate on the same context.
            Q_ASSERT(context == self->context);

            // eol is true when we got the last message. In this case, we only
            // expect to get one message, so this means that this function will
            // be called twice: once with eol false, and once with eol true. I
            // could just ignore the second call when eol is true and notify
            // during the first call, but I'm not sure how libpulse behaves when
            // multiple sources are queried at the same time, so I think it's
            // safer if I notify here, to keep the code the same as when
            // receiving multiple sources.
            if (eol) {
              Q_EMIT self->devicesChanged();
              return;
            }

            auto devInfo = std::make_shared<PulseAudioDeviceInfo>(info);

            auto oldInfo =
                std::find_if(self->m_devices.begin(), self->m_devices.end(),
                             [info](auto elem) {
                               return elem.get()->pulseIndex() == info->index;
                             });

            if (oldInfo == self->m_devices.end()) {
              qDebug()
                  << "Received an update for a source that is not in the list";

              // Since the source does not exist in the list, just insert it.
              self->m_devices.push_back(devInfo);
            } else {
              // Replace the old element with the new device info.
              *oldInfo = devInfo;
            }
          },
          self);
      break;

    case PA_SUBSCRIPTION_EVENT_REMOVE:
      // A source was removed.
      // We need to find the source in the list and remove it.
      std::erase_if(self->m_devices, [idx](auto info) {
        return info.get()->pulseIndex() == idx;
      });

      Q_EMIT self->devicesChanged();

      break;
    }

    break;

  default:
    qFatal() << "Unexpected event facility received";
    return;
  }
}

void AudioWorker::scanDevices() {
  // First, clean the list of existing devices.
  this->m_devices.clear();

  // Then, call the operation that receives the source devices from the server.
  // This operation runs asynchronously.
  pa_operation *get_sources_operation = pa_context_get_source_info_list(
      this->context,
      [](pa_context *context, const pa_source_info *info, int eol, void *data) {
        AudioWorker *self = static_cast<AudioWorker *>(data);

        // Make sure the callback comes from the same object.
        Q_ASSERT(context == self->context);

        // If eol is true, then that means that we reached the end of the list.
        // The end of the list is exclusive, meaning the last element was
        // provided to the previous callback call. In this call, info is null.
        if (eol) {
          Q_EMIT self->devicesChanged();
          return;
        }

        self->m_devices.push_back(std::make_unique<PulseAudioDeviceInfo>(info));
      },
      this);

  pa_operation_unref(get_sources_operation);
}

void AudioWorker::onReady() {
  // Set the event callback.
  pa_context_set_subscribe_callback(
      this->context, &AudioWorker::subscribedEventCallback, this);

  // Subscribe to events.
  pa_subscription_mask_t mask =
      (pa_subscription_mask_t)(PA_SUBSCRIPTION_MASK_SOURCE |
                               PA_SUBSCRIPTION_MASK_SINK);
  pa_context_subscribe(this->context, mask, nullptr, nullptr);

  // Start by scanning all available devices.
  this->scanDevices();
}

void AudioWorker::run() {
  int ret;

  ret = pa_context_connect(this->context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
  if (ret < 0) {
    this->m_errorMessage =
        QStringLiteral("Could not connect to PulseAudio server: ");
    const char *pulseError = pa_strerror(pa_context_errno(this->context));
    this->m_errorMessage.append(QString::fromUtf8(pulseError));
    Q_EMIT this->error(this->m_errorMessage);
    return;
  }

  qDebug() << "Connecting to PulseAudio server...";

  // TODO: Handle the return value of this.
  pa_mainloop_run(this->loop, nullptr);
}

void AudioWorker::onCurrentDeviceChange() {
  int ret;

  // If there is a previous stream active, clean it up.
  if (this->stream != nullptr) {
    ret = pa_stream_disconnect(this->stream);
    if (ret) {
      qWarning() << "Failed to disconnect PulseAudio stream from server";
    }

    // Clean up the stream object now that we don't need it
    pa_stream_unref(this->stream);
    this->stream = nullptr;
  }

  // TODO: Don't pull these out of our asses.
  pa_sample_spec sample_spec = {
      .format = PA_SAMPLE_U8,
      .rate = 44100,
      .channels = 2,
  };

  // TODO: Don't pull these out of our asses.
  pa_channel_map channel_map = {
      .channels = 2,
      .map = {PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT},
  };

  // TODO: Stream with proplist
  this->stream = pa_stream_new(this->context, "xyz.flafflar.panon",
                               &sample_spec, &channel_map);

  pa_stream_set_state_callback(
      this->stream,
      [](pa_stream *stream, void *data) {
        // CAUTION: The stream in self might not be the same stream that called
        // this callback, because there is a possibility that the callback was
        // invoked for an old stream that didn't yet got cleaned up.
        AudioWorker *self = static_cast<AudioWorker *>(data);

        pa_stream_state_t state = pa_stream_get_state(stream);

        switch (state) {
        case PA_STREAM_UNCONNECTED:
          break;

        case PA_STREAM_CREATING:
          qDebug() << "Creating stream...";
          break;

        case PA_STREAM_READY:
          qDebug() << "The stream is ready";
          self->startRecording();
          break;

        case PA_STREAM_FAILED:
          qDebug() << "The stream failed";
          break;

        case PA_STREAM_TERMINATED:
          qDebug() << "The stream was terminated";
          break;
        }
      },
      this);

  const char *const deviceName =
      this->m_currentDevice->identifier().toLocal8Bit().data();

  // TODO: Replace the nullptrs with values.
  // TODO: Do something with the return value of this.
  pa_stream_connect_record(this->stream, deviceName, nullptr,
                           PA_STREAM_NOFLAGS);
}

void AudioWorker::streamCallback(pa_stream *stream, size_t nbytes, void *data) {
  AudioWorker *self = static_cast<AudioWorker *>(data);

  // Make sure we are operating on the current stream.
  if (self->stream != stream) {
    qWarning() << "Stream data callback got called on old stream";
    return;
  }

  qDebug() << nbytes;
}

void AudioWorker::setFps(uint32_t fps) { this->m_fps = fps; }

void AudioWorker::updateStreamFps() {
  if (this->stream == nullptr)
    return;

  // This operation only works on running streams, so if the stream is not
  // ready, return.
  if (pa_stream_get_state(this->stream) != PA_STREAM_READY)
    return;

  // We need this to get the stream's sample rate.
  const pa_sample_spec *sample_spec = pa_stream_get_sample_spec(this->stream);

  uint32_t fragsize = pa_bytes_per_second(sample_spec) / this->m_fps;

  // The -1s here mean that we let the server decide on an appropriate size. The
  // only size we care aboud is the fragment size, which is how many bytes of
  // data the server will send us in each packet.
  pa_buffer_attr buffer_attr = {.maxlength = (uint32_t)-1,
                                .tlength = (uint32_t)-1,
                                .prebuf = (uint32_t)-1,
                                .minreq = (uint32_t)-1,
                                .fragsize = fragsize};

  pa_stream_set_buffer_attr(this->stream, &buffer_attr, nullptr, nullptr);
}

void AudioWorker::startRecording() {
  // Make sure the stream receives data at the correct fps.
  this->updateStreamFps();

  // Set a callback that is called every time a new fragment of audio is
  // available.
  pa_stream_set_read_callback(
      this->stream,
      [](pa_stream *stream, size_t nbytes, void *data) {
        AudioWorker *self = static_cast<AudioWorker *>(data);

        // Make sure we are operating on the active stream.
        if (stream != self->stream) {
          qWarning() << "Received audio data on stale stream";

          // Unset the callback of the stream so we don't receive any other
          // updates for it.
          pa_stream_set_read_callback(stream, nullptr, nullptr);

          return;
        }

        // TODO: Who deallocates this?
        uint8_t *buffer;
        size_t size;

        // Read the raw data into the buffer.
        int ret = pa_stream_peek(stream, (const void **)&buffer, &size);
        if (ret < 0) {
          qWarning() << "Failed to read data from stream, skipping...";
          return;
        }

        // There are three possibilites here: either the buffer contains data,
        // is empty, or has a hole.
        // If the buffer is empty, *buffer will be null and size will be 0.
        // If the buffer has a hole, *buffer will be null and size will be
        // nonzero.
        // If the buffer has data, *buffer will be non-null.
        if (buffer == nullptr) {
          if (size == 0) {
            // The buffer should not be empty, since we got a callback that it
            // contains data.
            qFatal() << "Buffer was unexpectedly empty";
            return;

          } else {
            // The buffer has a hole that we need to consume.
            ret = pa_stream_drop(stream);
            if (ret != 0) {
              qWarning() << "Failed to drop buffer";
            }
            return;
          }
        }

        // TODO: I'm not sure how to handle that.
        Q_ASSERT(size == nbytes);

        // Now, we can start processing the data.

        // We have two channels, so the number of samples is half the number of
        // bytes.
        size_t nsamples = size / 2;

        // First, resize the old buffers in case the fps changed.
        self->bufferLeft.resize(nsamples);
        self->bufferRight.resize(nsamples);

        // Separate the interleaved samples into the two separate buffers.
        for (size_t i = 0; i < nsamples; i += 1) {
          self->bufferLeft[i] = buffer[2 * i];
          self->bufferRight[i] = buffer[2 * i + 1];
        }

        // Now that we have read the buffer from the server, we should instruct
        // the server to drop it.
        ret = pa_stream_drop(stream);
        if (ret != 0) {
          // TODO: I'm not sure how to handle this.
          qWarning() << "Failed to drop buffer";
        }

        Q_EMIT self->buffersUpdated();
      },
      this);
}
