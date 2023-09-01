#!/usr/bin/env sh

set -xe

CFLAGS="-std=c99 -Wall -Wextra -pedantic `pkg-config --cflags raylib` -lm"
LIBS="-I./vendor/raylib/src -lraylib"

mkdir -p build
gcc $CFLAGS ./main.c -o ./build/physics $LIBS
