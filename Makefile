CC = g++

CFLAGS = -Wall

LDFLAGS =

OBJECTS = main.o fish.o site.o tank.o
FILES = main.cc fish.cc site.cc tank.cc

TARGET = fish_tank

all: $(FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LDFLAGS)

.PHONY: test clean

test: all
	./$(TARGET) -m 95000 -t 350 -s 17

clean:
	$(RM) $(OBJECTS) $(TARGET)
