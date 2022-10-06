#!/bin/sh


cc='gcc' 
flags='-Wall -Wextra -pedantic-errors -std=c99'
sources='forward_list.c main.c'

cc ${flags} ${sources}



