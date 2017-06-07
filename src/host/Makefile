CC=gcc
CFLAGS= -g -Wall
LF = `pkg-config fluidsynth --libs`

all: host_serial midi_out

host_serial:
	$(CC) $(CFLAGS) -o host_serial host_serial.c $(LF)

midi_out:
	$(CC) $(CFLAGS) -o midi_out midi_out.c $(LF)

clean:
	$(RM) *.o *~ 
