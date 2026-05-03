CC = gcc
CFLAGS = -Wall -g -Iobj -Isrc -Wno-unused-function

SRC = src/
OBJ = obj/
BIN = bin/

LEXER = lexeur
PARSER = grammar
TREE = ast
SYMBOL = symbol_table

OBJS = $(OBJ)$(LEXER).o \
       $(OBJ)$(PARSER).o \
       $(OBJ)$(TREE).o \
       $(OBJ)$(SYMBOL).o

TARGET = $(BIN)tpcas

all: $(TARGET)

$(TARGET): $(BIN) $(OBJ) $(OBJS)
	$(CC) -o $@  $(OBJS)

# Compilation générique des .c -> .o
$(OBJ)%.o: $(OBJ)%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Bison
$(OBJ)$(PARSER).c $(OBJ)$(PARSER).h: $(SRC)$(PARSER).y
	bison -d -o $(OBJ)$(PARSER).c $<

# Flex
$(OBJ)$(LEXER).c: $(SRC)$(LEXER).l $(OBJ)$(PARSER).h
	flex -o $@ $<

# Sources copiés / générés
$(OBJ)$(TREE).c: $(SRC)$(TREE).c
	cp $< $@

$(OBJ)$(SYMBOL).c: $(SRC)$(SYMBOL).c
	cp $< $@

# Dossiers
$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

# Clean
clean:
	rm -f $(OBJ)*.o $(OBJ)*.c $(OBJ)*.h $(BIN)tpcas

.PHONY: all clean test
test:
	./testing.sh
