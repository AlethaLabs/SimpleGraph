CC = clang
CFLAGS = -Wall -Wextra -std=c11
RAYLIB_INC = -I/usr/local/opt/raylib/include
RAYLIB_LIB = -L/usr/local/opt/raylib/lib
LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreAudio

TARGET = graph
SRC = graph.c

.PHONY: all run clean rebuild

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(RAYLIB_INC) $(RAYLIB_LIB) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all
