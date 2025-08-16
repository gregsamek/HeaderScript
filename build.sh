#!/bin/sh

set -e

(
    mkdir -p obj

    # Determine the number of available CPU cores
    if command -v nproc >/dev/null; then
        JOBS=$(nproc)
    elif command -v sysctl >/dev/null; then
        JOBS=$(sysctl -n hw.ncpu)
    else
        JOBS=2
    fi

    find src -name "*.c" | xargs -P "$JOBS" -I {} sh -c '
        source_file="$1"
        echo "Compiling $source_file"
        clang -c "$source_file" -o "obj/$(basename "$source_file" .c).o" \
            -I/usr/local/include \
            -fcolor-diagnostics -fansi-escape-codes \
            -g -O0 \
            -fno-omit-frame-pointer
    ' _ {}

    echo "Linking..."
    clang obj/*.o -o \
        build/main \
        -g # Keep -g on the link line to tell the linker to preserve symbols

    echo "Generating dSYM file..."
    dsymutil build/main

    rm -rf obj

    echo "Build complete."
)