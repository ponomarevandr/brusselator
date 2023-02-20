#!/usr/bin/env bash

echo "==================== Checking build directory..."
cd "$(dirname $0)"
mkdir -p build
cd build
echo "==================== Running the configuring script of Root..."
if [ ! -f root_path.txt ];
then
	echo "Enter the path to the folder of Root:"
	read -r ROOT_PATH
	echo "${ROOT_PATH}" > root_path.txt
fi
read -r ROOT_PATH < root_path.txt
echo "From folder ${ROOT_PATH}"
source "${ROOT_PATH}/bin/thisroot.sh"
if [ $? -ne 0 ]; then
	exit 1
fi
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