EXE = main
CFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -g
SRC = $(wildcard *.c) $(wildcard */*.c)

$(EXE): $(SRC)
	gcc $(CFLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE)
