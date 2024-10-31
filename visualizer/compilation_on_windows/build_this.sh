#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)/.."
mkdir -p build
cd build

../compilation_on_windows/auxiliary/set_root_dir.sh
read -r ROOT_DIR < root_path.txt
export ROOT_DIR

../compilation_on_windows/auxiliary/set_fltk_dir.sh
read -r FLTK_DIR < fltk_path.txt
FLTK_DIR="${FLTK_DIR}/build/"
export FLTK_DIR

../compilation_on_windows/auxiliary/set_matheval_dir.sh
read -r MATHEVAL_DIR < matheval_path.txt
export MATHEVAL_DIR

echo "==================== Replacing existing CMakeLists.txt..."
rm ../CMakeLists.txt
cp ../compilation_on_windows/auxiliary/cmakelists_this.txt ../CMakeLists.txt

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