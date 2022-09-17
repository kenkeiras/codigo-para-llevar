.PHONY=all test clean

CC?=gcc

all: check-halt test

check-halt: check-halt.c
	$(CC) $< -o $@


test: check-halt
	bash test.sh
