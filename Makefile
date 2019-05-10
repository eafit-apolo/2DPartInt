.DEFAULT_GOAL := all

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
CXXFLAGS            =
COMMON_FLAGS        = -O3 -Wall -Wextra -Werror
ALL_CFLAGS          = -std=c11 $(COMMON_FLAGS) $(CFLAGS)
ALL_CXXFLAGS        = -std=c++11 $(COMMON_FLAGS) $(CXXFLAGS)
RM                  = rm -rf
MKDIR               = mkdir -p

$(BUILD_DIR)/functions.o: $(SRC_C_DIR)/functions.c $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BUILD_DIR)/config.o: $(SRC_CXX_DIR)/config.cpp $(INC_DIR)/config.h
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(ALL_CXXFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BUILD_DIR)/main.o: $(SRC_CXX_DIR)/main.cpp $(INC_DIR)/config.h $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(ALL_CXXFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BIN_DIR)/$(PROGRAM_NAME): $(BUILD_DIR)/config.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/main.o
	$(MKDIR) $(BIN_DIR)
	$(CXX) $(ALL_CXXFLAGS) -o $@ $^ -lm

.PHONY: all
all: $(BIN_DIR)/$(PROGRAM_NAME)

$(BUILD_DIR)/functions_spec.o: $(TEST_DIR)/functions_spec.c $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BIN_DIR)/functions_spec: $(BUILD_DIR)/functions.o $(BUILD_DIR)/functions_spec.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: test
test: $(BIN_DIR)/functions_spec

.PHONY: clean
clean:
	$(RM) $(BIN_DIR)
	$(RM) $(BUILD_DIR)
