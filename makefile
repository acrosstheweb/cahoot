EXE = main
SRC = $(wildcard */*.c)
OBJ = $(SRC:.c=.o)

$(EXE): $(OBJ)
    gcc -o $(EXE) $(OBJ)

%.o: %.c
    gcc -c $<

clean:
    rm -f $(EXE) $(OBJ)
