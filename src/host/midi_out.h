#ifndef BLUETOOTHMIDIHOODIE_MIDI_OUT_H
#define BLUETOOTHMIDIHOODIE_MIDI_OUT_H

#include <unistd.h>
#include <fluidsynth.h>
//#include "RtMidi.h"
//#include "midi_play.h"

#define SOUNDFONT "test.sf2"

fluid_synth_t* synth;
fluid_audio_driver_t* driver;
fluid_settings_t* settings;

void create_synth(void);
void play_note(int, int, int);
void release_note(int, int);
void delete_synth(void);

#endif //BLUETOOTHMIDIHOODIE_MIDI_OUT_H
