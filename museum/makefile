# Jozef Zivcic PV112 project

CC = g++
CC_FLAGS = -w -std=c++11 -Wall -Wextra
L_FLAGS = -lGL -lglut -lGLEW -lIL

EXEC = museum
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(L_FLAGS)

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
