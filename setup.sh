#!/usr/bin/env bash

set -xe

mkdir -p vendor

# Add Raylib to ./vendor
git clone --depth 1 "https://github.com/raysan5/raylib" vendor/raylib
cd vendor/raylib/src
make PLATFORM=PLATFORM_DESKTOP
