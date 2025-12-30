#!/bin/bash

gcc -ansi -Wall -Wextra -Wpedantic -Werror src/cll.c src/doc.c src/lineeditor.c -o out/lineeditor.out

if [ $? -eq 0 -a $1 -eq 1 ]; then
	./out/lineeditor.out
fi
