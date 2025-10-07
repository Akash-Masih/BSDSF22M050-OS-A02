# Makefile for BSDSF22M050-OS-A02 (Feature 3)
CC = gcc
SRC_DIR = src
BIN_DIR = bin
CFLAGS = -Wall -Wextra -std=c99

SRC = $(SRC_DIR)/ls-v1.2.0.c
OUT = $(BIN_DIR)/ls-v1.2.0

# Default target
all: $(OUT)

# Build rule
$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	@echo "✅ Build complete: $(OUT)"

# Run the compiled binary
run:
	@./$(OUT) -l

# Clean compiled files
clean:
	@rm -f $(BIN_DIR)/*
	@echo "🧹 Clean complete."

# Targets for previous versions
v1.0:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.0.0.c -o $(BIN_DIR)/ls-v1.0.0
	@echo "✅ Built version 1.0.0"

v1.1:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.1.0.c -o $(BIN_DIR)/ls-v1.1.0
	@echo "✅ Built version 1.1.0"

v1.2:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.2.0.c -o $(BIN_DIR)/ls-v1.2.0
	@echo "✅ Built version 1.2.0"
