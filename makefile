CC = gcc
CFLAGS = -Wall -Wextra -I/usr/local/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -g

SRC_DIR = .
LIB_DIR = lib

SOURCES = $(wildcard $(SRC_DIR)/main.c) $(wildcard $(LIB_DIR)/*.c) $(wildcard $(SRC_DIR)/vue/*.c)

IMG_SRC = $(wildcard $(SRC_DIR)/img/*)
IMG_OBJ = $(patsubst $(SRC_DIR)/img/%, $(LIB_DIR)/images_%.c, $(IMG_SRC))

TARGET = main

$(LIB_DIR)/images.c: $(IMG_SRC)
	gdk-pixbuf-csource --raw --name=images $(IMG_SRC) > $@

$(TARGET): $(SOURCES) $(IMG_OBJ)
	$(CC) $(CFLAGS) $(SOURCES) $(IMG_OBJ) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(IMG_OBJ)
