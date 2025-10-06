# Makefile for BSDSF22M050-OS-A02

CC = gcc
SRC_DIR = src
BIN_DIR = bin
CFLAGS = -Wall -Wextra -std=c99

# Default target
all: $(BIN_DIR)/ls-v1.4.0

# Build rules
$(BIN_DIR)/ls-v1.0.0: $(SRC_DIR)/ls-v1.0.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.0.0.c -o $(BIN_DIR)/ls-v1.0.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.0.0"

$(BIN_DIR)/ls-v1.1.0: $(SRC_DIR)/ls-v1.1.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.1.0.c -o $(BIN_DIR)/ls-v1.1.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.1.0"

$(BIN_DIR)/ls-v1.2.0: $(SRC_DIR)/ls-v1.2.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.2.0.c -o $(BIN_DIR)/ls-v1.2.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.2.0"

$(BIN_DIR)/ls-v1.3.0: $(SRC_DIR)/ls-v1.3.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.3.0.c -o $(BIN_DIR)/ls-v1.3.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.3.0"

$(BIN_DIR)/ls-v1.4.0: $(SRC_DIR)/ls-v1.4.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/ls-v1.4.0.c -o $(BIN_DIR)/ls-v1.4.0
	@echo "✅ Build complete: $(BIN_DIR)/ls-v1.4.0"

# Run the latest version by default
run:
	./$(BIN_DIR)/ls-v1.4.0

# Clean binaries
clean:
	rm -f $(BIN_DIR)/*

# Version-specific targets
v1.0: $(BIN_DIR)/ls-v1.0.0
v1.1: $(BIN_DIR)/ls-v1.1.0
v1.2: $(BIN_DIR)/ls-v1.2.0
v1.3: $(BIN_DIR)/ls-v1.3.0
v1.4: $(BIN_DIR)/ls-v1.4.0
