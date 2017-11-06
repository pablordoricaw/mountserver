# Makefile for the mount_c executable
C = mount_c

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O3

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONEY: all
all: $(C)

$(C): mount.o mount.c
	$(CC) $(CFLAGS) mount.o -o $@

.PHONEY: help
help:
	@echo "make options: all, help"

.PHONEY: clean
clean:
	rm -fv $(C) *.o
