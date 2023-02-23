#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)"
mkdir -p build
cd build
echo "==================== Setting path to Root..."
if [ ! -f root_path.txt ];
then
	echo "Enter the path to the folder of Root:"
	read -r ROOT_FOLDER
	echo "${ROOT_FOLDER}" > root_path.txt
fi
read -r ROOT_FOLDER < root_path.txt
echo "From folder ${ROOT_FOLDER}"
ROOT_DIR="${ROOT_FOLDER}/cmake"
export ROOT_DIR
echo "==================== Setting path to FLTK..."
if [ ! -f fltk_path.txt ];
then
	echo "Enter the path to the folder of FLTK:"
	read -r FLTK_FOLDER
	echo "${FLTK_FOLDER}" > fltk_path.txt
fi
read -r FLTK_FOLDER < fltk_path.txt
echo "From folder ${FLTK_FOLDER}"
FLTK_DIR="${FLTK_FOLDER}/build"
export FLTK_DIR
echo "==================== Running CMake..."
cmake ..
if [ $? -ne 0 ]; then
	exit 1
fi
echo "==================== Running Make..."
make
if [ $? -ne 0 ]; then
	exit 1
fi