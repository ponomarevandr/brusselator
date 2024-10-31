#!/usr/bin/env bash

echo "==================== Setting path to Matheval..."
if [ ! -f matheval_path.txt ];
then
	echo "Enter the path to the folder of Matheval:"
	read -r MATHEVAL_DIR
	echo "${MATHEVAL_DIR}" > matheval_path.txt
fi
read -r MATHEVAL_DIR < matheval_path.txt
echo "From folder ${MATHEVAL_DIR}"