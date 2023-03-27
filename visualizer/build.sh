#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)"
mkdir -p build
cd build

echo "==================== Setting path to Root..."
if [ ! -f root_path.txt ];
then
	echo "Enter the path to the folder of Root:"
	read -r ROOT_DIR
	echo "${ROOT_DIR}" > root_path.txt
fi
read -r ROOT_DIR < root_path.txt
echo "From folder ${ROOT_DIR}"
export ROOT_DIR

echo "==================== Setting path to FLTK..."
if [ ! -f fltk_path.txt ];
then
	echo "Enter the path to the folder of FLTK:"
	read -r FLTK_DIR
	echo "${FLTK_DIR}" > fltk_path.txt
fi
read -r FLTK_DIR < fltk_path.txt
echo "From folder ${FLTK_DIR}"
FLTK_DIR="${FLTK_DIR}/build"
export FLTK_DIR

echo "==================== Running CMake..."
echo "With custom flags $1"
CUSTOM_FLAGS="$1"
export CUSTOM_FLAGS
cmake ..
if [ $? -ne 0 ]; then
	exit 1
fi

echo "==================== Running Make..."
make
if [ $? -ne 0 ]; then
	exit 1
fi