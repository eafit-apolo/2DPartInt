.DEFAULT_GOAL := all

PROGRAM_NAME        = 2DpartInt
SRC_DIR             = src
BIN_DIR             = bin
OBJ_DIR             = obj
INC_DIR             = include
TEST_DIR            = test
CC                  = gcc
CFLAGS              = 
ALL_CFLAGS          = -std=c11 -O3 -Wall -Wextra -Werror $(CFLAGS)
RM                  = rm -rf
MKDIR               = mkdir -p

$(OBJ_DIR)/functions.o: $(SRC_DIR)/functions.c $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(OBJ_DIR)/functions_spec.o: $(TEST_DIR)/functions_spec.c $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BIN_DIR)/functions_spec: $(OBJ_DIR)/functions.o $(OBJ_DIR)/functions_spec.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: functions_spec
functions_spec: $(BIN_DIR)/functions_spec
	$(BIN_DIR)/functions_spec

.PHONY: test
test: functions_spec

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/data.h $(INC_DIR)/functions.h
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(ALL_CFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BIN_DIR)/$(PROGRAM_NAME): $(OBJ_DIR)/functions.o $(OBJ_DIR)/main.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: all
all: $(BIN_DIR)/$(PROGRAM_NAME)

.PHONY: run
run: all
	$(BIN_DIR)/$(PROGRAM_NAME)

.PHONY: clean
clean:
	$(RM) $(BIN_DIR)
	$(RM) $(OBJ_DIR)
