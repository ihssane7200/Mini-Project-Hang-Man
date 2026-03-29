# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -I./raylib/include

# Linker flags (Adjusted for Windows MinGW and Raylib)
LDFLAGS = -L./raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Target executable name
TARGET = hangman.exe

# Source files
SRC = src/main.c src/msvcrt_compat.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	del $(TARGET)
