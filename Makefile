CC = gcc
CFLAGS = -E -C -P
INCLUDE = -I./inc
SRC = ./src/main.c
OUTPUT = ./output/output.c

.PHONY: preprocess

preprocess:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(OUTPUT)
