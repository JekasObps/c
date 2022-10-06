#!/bin/sh
gcc -O3 -g -std=c99 -c node.c -o node.o
gcc -O3 -g -std=c99 -c dlist.c -o dlist.o
gcc -O3 -g -std=c99 -c dlist_iter.c -o dlist_iter.o
gcc -O3 -g -std=c99 -c slist.c -o slist.o
gcc -O3 -g -std=c99 -c slist_iter.c -o slist_iter.o
ar rfc slist.a slist.o slist_iter.o node.o
ar rfc dlist.a dlist.o dlist_iter.o node.o 
gcc -O3 -g main.c slist.a dlist.a

