# Makefile for all LS versions (v1.0.0 -> v1.5.0)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC_DIR = src
BIN_DIR = bin

V1_0 = $(BIN_DIR)/ls-v1.0.0
V1_1 = $(BIN_DIR)/ls-v1.1.0
V1_2 = $(BIN_DIR)/ls-v1.2.0
V1_3 = $(BIN_DIR)/ls-v1.3.0
V1_4 = $(BIN_DIR)/ls-v1.4.0
V1_5 = $(BIN_DIR)/ls-v1.5.0

all: $(V1_0) $(V1_1) $(V1_2) $(V1_3) $(V1_4) $(V1_5)

$(BIN_DIR)/ls-v1.0.0: $(SRC_DIR)/ls-v1.0.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/ls-v1.1.0: $(SRC_DIR)/ls-v1.1.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/ls-v1.2.0: $(SRC_DIR)/ls-v1.2.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/ls-v1.3.0: $(SRC_DIR)/ls-v1.3.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/ls-v1.4.0: $(SRC_DIR)/ls-v1.4.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/ls-v1.5.0: $(SRC_DIR)/ls-v1.5.0.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L $< -o $@

clean:
	rm -rf $(BIN_DIR)
