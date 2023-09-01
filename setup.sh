#!/usr/bin/env bash

set -xe

mkdir -p vendor

# Add Raylib to ./vendor
git clone --depth 1 "https://github.com/raysan5/raylib" vendor/raylib
mkdir -p lib/raylib
mkdir -p vendor/raylib/build && cd vendor/raylib/build 
cmake -DCMAKE_INSTALL_PREFIX=../../../lib/raylib ..
make -j5
make install
