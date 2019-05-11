###############################################################################
# Global variables

PROGRAM_NAME        = 2DpartInt
SRC_C_DIR           = src/c
SRC_CXX_DIR         = src/cpp
BIN_DIR             = bin
BUILD_DIR           = build
INC_DIR             = include
TEST_DIR            = test
CC                  = gcc
CXX                 = g++
CFLAGS              =
ALL_CFLAGS          = -std=c11 -O3 -Wall -Wextra -Werror $(CFLAGS)
LDFLAGS		    = -lm
RM                  = rm -rf
MKDIR               = mkdir -p

###############################################################################
# Main program compilation

.DEFAULT_GOAL := all

.PHONY: all
all: $(BIN_DIR)/$(PROGRAM_NAME)

$(BIN_DIR)/$(PROGRAM_NAME): $(BUILD_DIR)/functions.o $(BUILD_DIR)/main.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/functions.o: $(SRC_C_DIR)/functions.c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BUILD_DIR)/main.o: $(SRC_C_DIR)/main.c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

###############################################################################
# Tests

.PHONY: test
test: $(BIN_DIR)/functions_spec
	$(BIN_DIR)/functions_spec

$(BIN_DIR)/functions_spec: $(BUILD_DIR)/functions.o $(BUILD_DIR)/functions_spec.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/functions_spec.o: $(TEST_DIR)/functions_spec.c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

###############################################################################
# Run

.PHONY: run
run: all
	$(BIN_DIR)/$(PROGRAM_NAME)

###############################################################################
# Clean

.PHONY: clean
clean:
	$(RM) $(BIN_DIR)
	$(RM) $(BUILD_DIR)
