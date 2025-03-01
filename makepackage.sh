#!/bin/bash

set -euo pipefail

#Verify the existence of third party files before packaging.
if [ ! -f "third_party/hsluv-glsl/hsluv-glsl.fsh" ];then
    echo "Cannot find third party files."
	git submodule update --init

fi

# Remove caches
rm ./plasmoid/contents/scripts/__pycache__/ -rf
rm ./plasmoid/contents/scripts/*/__pycache__/ -rf
rm ./plasmoid/contents/scripts/*/*/__pycache__/ -rf
rm ./plasmoid/contents/scripts/*/*/*/__pycache__/ -rf
rm ./panon.plasmoid -f

# i18n
mkdir -p build-translations
cd build-translations
cmake ../translations
make install DESTDIR=../plasmoid/contents/locale
cd ..

mkdir -p build
cd build
CMAKE_INSTALL_PREFIX="../" cmake ..
make install
cd ..

zip -r panon.plasmoid ./plasmoid 
