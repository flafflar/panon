#!/bin/bash
set -euo pipefail

./makepackage.sh

if [ -f "third_party/hsluv-glsl/hsluv-glsl.fsh" ];then
    kpackagetool6 -t Plasma/Applet --install ./build/dist/plasmoid || \
      kpackagetool6 -t Plasma/Applet --upgrade ./build/dist/plasmoid
else
    echo "Cannot find third party files."
fi
