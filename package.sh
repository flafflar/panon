#!/bin/bash

distro="$1"

echo "Building docker image..."
case ${distro} in
kubuntu25.10)
	docker_image=$(docker build -q docker/kubuntu25.10)
	;;
esac

echo "Running the build script"
docker run -it "${docker_image}" \
	--mount
bash
