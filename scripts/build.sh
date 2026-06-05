#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
OUT_EXE="$BUILD_DIR/red"

GLFW_CFLAGS=""
GLFW_LIBS=""
if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists glfw3; then
    GLFW_CFLAGS="$(pkg-config --cflags glfw3)"
    GLFW_LIBS="$(pkg-config --libs glfw3)"
else
    GLFW_LIBS="-lglfw -lGLEW -lGL -ldl -lm -lpthread"
fi

mkdir -p "$BUILD_DIR"

"$ROOT_DIR/scripts/init.sh"

g++ -std=c++17 -Wall -Wextra \
    $GLFW_CFLAGS \
    -I"$ROOT_DIR/src" \
    -c "$ROOT_DIR/src/main.cpp" \
    -o "$BUILD_DIR/main.o"

g++ -o "$OUT_EXE" \
    "$BUILD_DIR/main.o" \
    $GLFW_LIBS \
    -lGLEW -lGL -ldl -lm -lpthread

echo "Build complete: $OUT_EXE"
