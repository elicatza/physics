#!/usr/bin/env sh

set -xe

CFLAGS="-std=c99 -Wall -Wextra -pedantic -lm"
LIBS="-I./vendor/raylib/src -L./vendor/raylib/src -lraylib"

mkdir -p build
gcc $CFLAGS ./main.c -o ./build/physics $LIBS
