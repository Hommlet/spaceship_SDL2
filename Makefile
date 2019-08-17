# ----------
# commands
# ----------
CC=g++
LD=g++

# ----------
# flags
# ----------
CPPFLAGS=--pedantic -Wall -W -Wno-unused-parameter
LDFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf

# ----------
# objects
# ----------
EXEC=spaceship
OBJECTS=main.o SpaceObject.o Texture.o

# ----------
# paths
# ----------
GAME_PATH=.
INCLUDES=$(GAME_PATH)

# ----------
# Game
# ----------
all: $(EXEC)

spaceship: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CPPFLAGS) $(LDFLAGS)

SpaceObject.o: SpaceObject.cpp SpaceObject.h
	$(CC) -c SpaceObject.cpp -o SpaceObject.o $(CPPFLAGS) $(LDFLAGS)

Texture.o: Texture.cpp Texture.h
	$(CC) -c Texture.cpp -o Texture.o $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
	rm $(EXEC)
	clear
