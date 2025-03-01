#!/bin/bash
set -euo pipefail

./build.sh

plasmoidviewer --applet ./build/dist/plasmoid/
