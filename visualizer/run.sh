#!/usr/bin/env bash

./build.sh
if [ $? -ne 0 ]; then
	exit 1
fi
echo "====================Executing program..."
./bin/visualizer
