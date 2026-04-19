CC = gcc
CFLAGS = -Wall -g -Iobj -Isrc -Wno-unused-function

PARSER = grammar
LEXER = lexeur

SRC = src/
OBJ = obj/
BIN = bin/

$(BIN)tpcas: $(BIN) $(OBJ) $(OBJ)$(LEXER).o $(OBJ)$(PARSER).o
	$(CC) -o $@ $(OBJ)$(LEXER).o $(OBJ)$(PARSER).o 

$(OBJ)%.o: $(OBJ)%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)$(PARSER).c $(OBJ)$(PARSER).h: $(SRC)$(PARSER).y
	bison   -d -o $(OBJ)$(PARSER).c $<

$(OBJ)$(LEXER).c: $(SRC)$(LEXER).l $(OBJ)$(PARSER).h
	flex -o $@ $<

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -f $(OBJ)*.o $(OBJ)*.c $(OBJ)*.h $(BIN)tpcas

.PHONY: test
test:
	./testing.sh