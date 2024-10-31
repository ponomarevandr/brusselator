#!/usr/bin/env bash

echo "==================== Setting path to FLTK..."
if [ ! -f fltk_path.txt ];
then
	echo "Enter the path to the folder of FLTK:"
	read -r FLTK_DIR
	echo "${FLTK_DIR}" > fltk_path.txt
fi
read -r FLTK_DIR < fltk_path.txt
echo "From folder ${FLTK_DIR}"