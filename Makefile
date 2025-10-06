# Makefile for BSDSF22M050-OS-A02 (Feature 4)

CC = gcc
SRC_DIR = src
BIN_DIR = bin
CFLAGS = -Wall -Wextra -std=c99

SRC = $(SRC_DIR)/ls-v1.3.0.c
OUT = $(BIN_DIR)/ls-v1.3.0

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	@echo "✅ Build complete: $(OUT)"

run:
	./$(OUT)

clean:
	rm -f $(BIN_DIR)/*

# targets for versions
v1.0: $(BIN_DIR)/ls-v1.0.0
v1.1: $(BIN_DIR)/ls-v1.1.0
v1.2: $(BIN_DIR)/ls-v1.2.0
v1.3: $(BIN_DIR)/ls-v1.3.0

$(BIN_DIR)/ls-v1.3.0: $(SRC_DIR)/ls-v1.3.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.3.0.c -o $(BIN_DIR)/ls-v1.3.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.3.0"
