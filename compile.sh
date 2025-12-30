#!/bin/bash

gcc -ansi -Wall -Wextra -Wpedantic -Werror src/cll.c src/doc.c src/utils.c src/lineeditor.c -o out/lineeditor.out

if [ $? -eq 0 -a "$1" = "1" ]; then
	./out/lineeditor.out
	rm temp*.c
	rm temp*.c.out
fi
