.DEFAULT_GOAL := all

program_name 		= main
SRCdir				= src
BINdir				= bin
OBJdir 				= $(BINdir)/obj
INCdir				= include
TESTdir 			= $(SRCdir)/test
CC 					= gcc
CFLAGS 				= 
ALL_CFLAGS			= -std=c11 -O3 -Wall -Wextra -Werror $(CFLAGS)
RM 					= rm -rf
MKDIR 				= mkdir -p

$(OBJdir)/functions.o: $(SRCdir)/functions.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(ALL_CFLAGS) -I$(INCdir) -o $@ -c $<

$(OBJdir)/functions_spec.o: $(TESTdir)/functions_spec.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(ALL_CFLAGS) -I$(INCdir) -o $@ -c $<

$(TESTdir)/functions_spec: $(OBJdir)/functions.o $(OBJdir)/functions_spec.o
	$(MKDIR) $(TESTdir)
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: functions_spec
functions_spec: $(TESTdir)/functions_spec
	$(TESTdir)/functions_spec

.PHONY: test
test: functions_spec

$(OBJdir)/main.o: $(SRCdir)/main.c $(INCdir)/data.h $(INCdir)/functions.h
	$(MKDIR) $(OBJdir)
	$(CC) $(ALL_CFLAGS) -I$(INCdir) -o $@ -c $<

$(BINdir)/$(program_name): $(OBJdir)/functions.o $(OBJdir)/main.o
	$(CC) $(ALL_CFLAGS) -o $@ $^ -lm

.PHONY: all
all: $(BINdir)/$(program_name)

.PHONY: run
run: all
	$(BINdir)/$(program_name)

.PHONY: clean
clean:
	$(RM) $(BINdir)
