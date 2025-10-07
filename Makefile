# Makefile for BSDSF22M050-OS-A02 (Feature 2)

CC = gcc
SRC_DIR = src
BIN_DIR = bin
CFLAGS = -Wall -Wextra -std=c99

SRC = $(SRC_DIR)/ls-v1.1.0.c
OUT = $(BIN_DIR)/ls-v1.1.0

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	@echo "✅ Build complete: $(OUT)"

run:
	./$(OUT) -l

clean:
	rm -f $(BIN_DIR)/*
