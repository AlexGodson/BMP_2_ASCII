CC=gcc -std=c99
INC = /Include
CFLAGS=-g -Wall -Werror
_SRC=src
SRC=$(wildcard $(_SRC)/*.c)
_OBJ=src/OBJ
OBJ=$(patsubst $(_SRC)/%.c, $(_OBJ)/%.o, $(SRC))
OUT=Asciify

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

$(_OBJ)/%.o: $(_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(OUT)
	./$(OUT) inputs/calm_bear.bmp
	./$(OUT) inputs/checker.bmp
	./$(OUT) inputs/japanese_scene.bmp
	./$(OUT) inputs/rose_pond.bmp
	./$(OUT) inputs/stencil_owl.bmp
	./$(OUT) inputs/test_err.txt

clear:
	rm $(OBJ)*
	rm $(OUT)

