CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/usr/local/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -g

SRC_DIR = .
IMG_DIR = img

SOURCES = $(wildcard $(SRC_DIR)/main.c) $(wildcard $(SRC_DIR)/lib/*.c) $(wildcard $(SRC_DIR)/vue/*.c) $(wildcard $(IMG_DIR)/*)

TARGET = main

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET)
