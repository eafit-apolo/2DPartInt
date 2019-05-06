.DEFAULT_GOAL := default

program_name 		= main
SRCdir				= src
BINdir				= bin
OBJdir 				= $(BINdir)/obj
INCdir				= include
TESTdir 			= $(SRCdir)/test
CC 					= gcc
CFLAGS 				= -std=c11 -O3 -Wall -Wextra -Werror
RM 					= rm -rf
MKDIR 				= mkdir -p

$(OBJdir)/functions.o: $(SRCdir)/functions.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(CFLAGS) -I$(INCdir) -o $@ -c $<

$(OBJdir)/functions_spec.o: $(TESTdir)/functions_spec.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(CFLAGS) -I$(INCdir) -o $@ -c $<

$(TESTdir)/functions_spec: $(OBJdir)/functions.o $(OBJdir)/functions_spec.o
	$(MKDIR) $(TESTdir)
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: functions_spec
functions_spec: $(TESTdir)/functions_spec
	$(TESTdir)/functions_spec

.PHONY: test
test: functions_spec

$(OBJdir)/main.o: $(SRCdir)/main.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(CFLAGS) -I$(INCdir) -o $@ -c $<

$(BINdir)/$(program_name): $(OBJdir)/functions.o $(OBJdir)/main.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: compile
compile: $(BINdir)/$(program_name)

.PHONY: run
run: compile
	$(BINdir)/$(program_name)

.PHONY: clean
clean:
	$(RM) $(BINdir)

.PHONY: default
default: compile
