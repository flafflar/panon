FROM ubuntu:25.10 AS build

RUN apt-get update 
RUN apt-get install -y git dpkg-dev debhelper git-buildpackage

WORKDIR /build/panon

# Hack: copy only the debian directory that contains the build dependencies
# before copying the project. This way, Docker won't rerun the installation step
# unless the debian directory changes.
COPY pkg/kubuntu25.10/debian/ debian/
RUN --mount=type=cache,target=/var/cache/apt \
  apt-get build-dep -y .

COPY . .

# Generate the new changelog for the debian package.
# This cannot be skipped, because the package's version is determined by the
# changelog.
ARG PANON_VERSION
ARG USER_NAME
ARG USER_EMAIL
RUN USER=${USER_NAME} EMAIL=${USER_EMAIL} gbp dch \
  --debian-branch=$(git rev-parse --abbrev-ref HEAD) \
  --debian-tag='v%(version)s' \
  --new-version=${PANON_VERSION}

RUN dpkg-buildpackage

# Make an empty image that only contains the build outputs.
FROM scratch
ARG PANON_VERSION
COPY --from=build \
  /build/panon-dbgsym_${PANON_VERSION}_amd64.ddeb \
  /build/panon_${PANON_VERSION}.dsc \
  /build/panon_${PANON_VERSION}.tar.gz \
  /build/panon_${PANON_VERSION}_amd64.buildinfo \
  /build/panon_${PANON_VERSION}_amd64.changes \
  /build/panon_${PANON_VERSION}_amd64.deb \
  /
