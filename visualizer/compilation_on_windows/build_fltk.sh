#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)/.."
mkdir -p build
cd build

../compilation_on_windows/auxiliary/set_fltk_dir.sh
read -r FLTK_DIR < fltk_path.txt

echo "==================== Checking build directory of FLTK..."
cd "${FLTK_DIR}"
mkdir -p build
cd build

echo "==================== Running CMake..."
cmake ..
if [ $? -ne 0 ]; then
	exit 1
fi

echo "==================== Building..."
cmake --build . --config Release
if [ $? -ne 0 ]; then
	exit 1
fi

echo "Done!"