CC = g++
CFLAGS = -Wall
INCLUDES = -Iheaders
SOURCES = src/board.cpp src/main.cpp src/player.cpp src/piece.cpp src/game.cpp
OBJ = $(SOURCES:.cpp=.o)
EXECUTABLE = fairy-chess

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECUTABLE)