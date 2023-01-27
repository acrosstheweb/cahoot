CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/usr/local/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/local/lib -lSDL2 -lm

SRC_DIR = .

SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)

TARGET = main

$(TARGET): $(SOURCES)
    $(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
    rm -f $(TARGET)