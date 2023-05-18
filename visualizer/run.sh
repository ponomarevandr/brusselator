#!/usr/bin/env bash

cd "$(dirname $0)"
./build.sh "$1"
if [ $? -ne 0 ]; then
	exit 1
fi
echo "====================Executing the program..."
cd bin
./visualizer