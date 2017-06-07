CC = gcc
CFLAGS = -g -Wall
OBJECT = serial
FILES = serial.c serial.h

all: $(OBJECT)

$(TARGET): $(FILES)
	$(CC) $(CFLAGS) -o $(OBJECT) $(FILES)

clean:
	$(RM) $(OBJECT) *.o *~


