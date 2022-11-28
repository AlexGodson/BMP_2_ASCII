CC=gcc -std=c99
INC = /Include
CFLAGS=-g -Wall -Werror
_SRC=src
SRC=$(wildcard $(_SRC)/*.c)
_OBJ=src/OBJ
OBJ=$(patsubst $(_SRC)/%.c, $(_OBJ)/%.o, $(SRC))
OUT=ASCIIFY

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

$(_OBJ)/%.o: $(_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OUT)
	./$(OUT)

clear:
	rm $(OBJ)*
	rm $(OUT)

