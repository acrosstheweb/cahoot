CC = gcc
CFLAGS = -Wall -Wextra -I/usr/local/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -g

SRC_DIR = .
IMG_DIR = img

IMG_SOURCES = $(wildcard $IMG_DIR/*)

$(IMG_OBJECTS): $(IMG_SOURCES)
	gdk-pixbuf-csource --name=images_data $(IMG_SOURCES) > ./lib/images.c

SOURCES = $(wildcard $(SRC_DIR)/main.c) $(wildcard $(SRC_DIR)/lib/*.c) $(wildcard $(SRC_DIR)/vue/*.c)

TARGET = main


$(TARGET): $(SOURCES) $(IMG_OBJECTS)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET)
