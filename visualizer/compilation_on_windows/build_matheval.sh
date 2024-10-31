#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)/.."
mkdir -p build
cd build

../compilation_on_windows/auxiliary/set_matheval_dir.sh
read -r MATHEVAL_DIR < matheval_path.txt

echo "==================== Setting up CMakeLists.txt for Matheval..."
cp ../compilation_on_windows/auxiliary/cmakelists_matheval.txt "${MATHEVAL_DIR}/CMakeLists.txt"

echo "==================== Checking build directory of Matheval..."
cd "${MATHEVAL_DIR}"
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