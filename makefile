CC=gcc -std=c99
INC = /Include
CFLAGS=-g -Wall #-Werror
_SRC=src
SRC=$(wildcard $(_SRC)/*.c)
_OBJ=src/OBJ
OBJ=$(patsubst $(_SRC)/%.c, $(_OBJ)/%.o, $(SRC))
_INPUT=inputs
IMGS=$(wildcard $(_INPUT)/*.bmp)
OUT=Asciify

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

$(_OBJ)/%.o: $(_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

testunit: $(OUT)
	./$(OUT) inputs/NatGeoFrog256.bmp

test: $(OUT)
	@for i in $(IMGS); \
	do \
		./$(OUT) $$i;\
	done;

clear:
	rm $(OBJ)*
	rm $(OUT)

