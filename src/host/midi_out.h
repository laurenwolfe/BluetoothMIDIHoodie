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
#define MIDI_PATH "/tmp/midi_fifo"
#define BUF_SIZE 255
#define SCALE 0x60
#define CMD_MASK 0xf0
#define CHANNEL 0
#define VELOCITY 100

fluid_synth_t* synth;
fluid_audio_driver_t* driver;
fluid_settings_t* settings;
int midi_fd, delay, n_read, note;
char buf[BUF_SIZE];

void create_synth(void);
void process_input(int);
void delete_synth(void);

#endif //BLUETOOTHMIDIHOODIE_MIDI_OUT_H
