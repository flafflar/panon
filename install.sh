#!/bin/bash
set -euo pipefail

./cleanbuild.sh

kpackagetool6 -t Plasma/Applet --install ./build/dist/plasmoid || \
  kpackagetool6 -t Plasma/Applet --upgrade ./build/dist/plasmoid
