#!/usr/bin/env bash

./build.sh
if [ $? -ne 0 ]; then
	exit 1
fi
echo "====================Executing the program..."
cd bin
./visualizer