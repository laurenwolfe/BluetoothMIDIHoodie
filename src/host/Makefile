CC=gcc
CFLAGS= -g -Wall
OBJECT = midi_out
FILES = midi_out.c
LF = `pkg-config fluidsynth --libs`

all: $(OBJECT)

$(OBJECT): $(OBJECT).o
	$(CC) $(CFLAGS) -o $(OBJECT) $(FILES) $(LF)

clean:
	$(RM) $(OBJECT) *.o *~
