#!/bin/sh
cc='gcc -Wall -Wextra --pedantic-errors -ansi'
sources='main.c binary_search.c algorithms.c'

${cc} ${sources}
