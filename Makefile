# ---------------------------------------------
# Makefile for BSDSF22M050-OS-A02
# Compiles ls-v1.0.0.c into bin/ls-v1.0.0
# ---------------------------------------------

# Compiler
CC = gcc

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Source and Output Files
SRC = $(SRC_DIR)/ls-v1.0.0.c
OUT = $(BIN_DIR)/ls-v1.0.0

# Flags
CFLAGS = -Wall -Wextra -std=c99

# Default target
all: $(OUT)

# Build rule
$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	@echo "✅ Build complete! Executable created at $(OUT)"

# Clean rule
clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/* 
	@echo "🧹 Cleaned all compiled files."

# Run rule
run: all
	@echo "🚀 Running program..."
	./$(OUT)

# Help rule
help:
	@echo "Available commands:"
	@echo "  make          → Compile the program"
	@echo "  make run      → Build & run the program"
	@echo "  make clean    → Remove binaries & object files"
	@echo "  make help     → Show this message"
