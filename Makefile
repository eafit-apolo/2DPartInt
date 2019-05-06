.DEFAULT_GOAL := all

MAIN                = main
SRC                 = src
BIN                 = bin
OBJ                 = $(BIN)/obj
INC                 = include
TEST                = $(SRC)/test
CC                  = gcc
CFLAGS              = 
ALL_CFLAGS          = -std=c11 -O3 -Wall -Wextra -Werror $(CFLAGS)
RM                  = rm -rf
MKDIR               = mkdir -p

$(OBJ)/functions.o: $(SRC)/functions.c $(INC)/data.h $(INC)/functions.h
	$(MKDIR) $(OBJ)
	$(CC) $(ALL_CFLAGS) -I$(INC) -o $@ -c $<

$(OBJ)/functions_spec.o: $(TEST)/functions_spec.c $(INC)/data.h $(INC)/functions.h
	$(MKDIR) $(OBJ)
	$(CC) $(ALL_CFLAGS) -I$(INC) -o $@ -c $<

$(TEST)/functions_spec: $(OBJ)/functions.o $(OBJ)/functions_spec.o
	$(MKDIR) $(TEST)
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: functions_spec
functions_spec: $(TEST)/functions_spec
	$(TEST)/functions_spec

.PHONY: test
test: functions_spec

$(OBJ)/main.o: $(SRC)/main.c $(INC)/data.h $(INC)/functions.h
	$(MKDIR) $(OBJ)
	$(CC) $(ALL_CFLAGS) -I$(INC) -o $@ -c $<

$(BIN)/$(MAIN): $(OBJ)/functions.o $(OBJ)/main.o
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: all
all: $(BIN)/$(MAIN)

.PHONY: run
run: all
	$(BIN)/$(MAIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
