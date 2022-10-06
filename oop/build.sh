#!/bin/sh


cc='gcc' 
flags='-Wall -Wextra -pedantic-errors'
sources='object.c animal.c cat.c dog.c main.c'

cc ${flags} ${sources}



