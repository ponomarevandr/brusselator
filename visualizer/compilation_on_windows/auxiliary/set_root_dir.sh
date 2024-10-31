#!/usr/bin/env bash

echo "==================== Setting path to Root..."
if [ ! -f root_path.txt ];
then
	echo "Enter the path to the folder of Root:"
	read -r ROOT_DIR
	echo "${ROOT_DIR}" > root_path.txt
fi
read -r ROOT_DIR < root_path.txt
echo "From folder ${ROOT_DIR}"