#!/bin/bash

MIN_SUPPORTED_DOTNET_VERSION="6"

if ! [ -x "$(command -v dotnet)" ]; then
  echo 'dotnet was not detected. Install the dotnet 6 SDK from https://dotnet.microsoft.com/download'
  exit 1
else
  dotnet_sdk_version=$(dotnet --version)
  dotnet_major_release=${dotnet_sdk_version::1}
  if [ "$dotnet_major_release" -lt "$MIN_SUPPORTED_DOTNET_VERSION" ]; then
    echo "The current version of dotnet is not supported. Install the dotnet 6 SDK from https://dotnet.microsoft.com/download"
    exit 1
  fi
fi
apt update
apt-get install flex
apt-get install bison
apt-get install ffmpeg
apt-get install gcc
hash -r