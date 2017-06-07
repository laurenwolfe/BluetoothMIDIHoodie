#ifndef BLUETOOTHMIDIHOODIE_MIDI_OUT_H
#define BLUETOOTHMIDIHOODIE_MIDI_OUT_H

#include <unistd.h>
#include <fluidsynth.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SOUNDFONT "test.sf2"
#define FIFO_PATH "/tmp/midi_fifo"
#define BUF_SIZE 255

fluid_synth_t* synth;
fluid_audio_driver_t* driver;
fluid_settings_t* settings;

void create_synth(void);
void play_note(int, int, int);
void release_note(int, int);
void delete_synth(void);

int midi_fd, delay, n_read;
char buf[BUF_SIZE];

#endif //BLUETOOTHMIDIHOODIE_MIDI_OUT_H
