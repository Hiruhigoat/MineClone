CC = gcc
CFLAGS = -std=c11 -g -Iinclude 
LDFLAGS = -luser32 -lgdi32 -lopengl32


SRCS = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))
TARGET = bin/mineClone.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist "bin" mkdir "bin"
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.c 
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-if exist build rmdir /S /Q build
	-if exist bin rmdir /S /Q bin