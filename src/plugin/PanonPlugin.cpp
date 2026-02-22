#include "PanonPlugin.h"

#include "AudioBackend.h"
#include "ShaderWidget.h"
#include "SpectrumTexture.h"
#include "WaveTexture.h"
#include "WaveViewer.h"

void PanonPlugin::registerTypes(const char *uri) {
  Q_ASSERT(QLatin1String(uri) == QLatin1String("xyz.flafflar.panon"));

  qmlRegisterType<SpectrumVolumeMode>(uri, 1, 0, "SpectrumVolumeMode");

  qmlRegisterType<AudioBackend>(uri, 1, 0, "AudioBackend");
  qmlRegisterType<ShaderWidget>(uri, 1, 0, "ShaderWidget");
  qmlRegisterType<SpectrumTexture>(uri, 1, 0, "SpectrumTexture");
  qmlRegisterType<WaveTexture>(uri, 1, 0, "WaveTexture");
  qmlRegisterType<WaveViewer>(uri, 1, 0, "WaveViewer");
}
