#!/usr/bin/env sh

set -xe

export PKG_CONFIG_PATH=lib/raylib/lib/pkgconfig/

CFLAGS="-std=c99 -Wall -Wextra -pedantic `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lm -lglfw -lpthread -ldl"

mkdir -p build
gcc $CFLAGS ./src/main.c -o ./build/physics $LIBS
