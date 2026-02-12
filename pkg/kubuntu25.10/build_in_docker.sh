#!/bin/bash

# cd to the project root
cd "$(realpath "$0")/../.."

docker build -t panon:kubuntu25.10 \
	--build-arg PANON_VERSION=$(./version.sh) \
	--build-arg USER_NAME=$(git config user.name) \
	--build-arg USER_EMAIL=$(git config user.email) \
	--output=pkg/kubuntu25.10/out \
	-f pkg/kubuntu25.10/Dockerfile .
