EXE = main
CFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -g
SRC = $(wildcard *.c) $(wildcard */*.c)
OBJ = $(SRC:.c=.o)

$(EXE): $(OBJ)
	gcc $(CFLAGS) -o $(EXE) $(OBJ)

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE) $(OBJ)