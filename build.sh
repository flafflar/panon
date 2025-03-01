#!/bin/bash

set -euo pipefail

#Verify the existence of third party files before packaging.
if [ ! -f "third_party/hsluv-glsl/hsluv-glsl.fsh" ];then
    echo "Cannot find third party files."
	git submodule update --init

fi

# Remove caches
find . -name __pycache__ -type d -exec rm -rf {} \; || true
rm ./build/dist/panon.plasmoid -f

mkdir -p build
cd build
CMAKE_INSTALL_PREFIX="./dist" cmake ..
make install
cd ..

ln -s ../../third_party ./build/dist/third_party

cd build/dist
zip -r panon.plasmoid ./plasmoid
echo 'SUCCESS! Created ./build/dist/panon.plasmoid'
