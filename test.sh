#!/bin/bash
set -euo pipefail

./build.sh

if [ -f "third_party/hsluv-glsl/hsluv-glsl.fsh" ];then
    plasmoidviewer --applet ./build/dist/plasmoid/
else
    echo "Cannot find third party files. Please execute: git submodule update --init"
fi
