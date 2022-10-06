#!/bin/sh

cc='gcc -Wall -Wextra -pedantic-errors'
sources='buble_sort.c main.c'

${cc} -g ${sources}
