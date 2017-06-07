#include "midi_out.h"

void create_synth() {
    settings = new_fluid_settings();

    fluid_settings_setstr(settings, "audio.driver", "coreaudio");

    synth = new_fluid_synth(settings);

    driver = new_fluid_audio_driver(settings, synth);

	if(!settings)
		printf("Settings NULL\n");

	if(!synth)
		printf("Synth NULL\n");

	if(!driver)
		printf("Driver NULL\n");


	//set soundfont
    int font = fluid_synth_sfload(synth, SOUNDFONT, 1);

	printf("font: %d\n", font);
}

void play_note(int channel, int key, int velocity) {
    int ret = fluid_synth_noteon(synth, channel, key, velocity);
	printf("return 1: %d\n", ret);
}

void release_note(int channel, int key) {
    int ret = fluid_synth_noteoff(synth, channel, key);
	printf("return 2: %d\n", ret);
}

void delete_synth() {
    delete_fluid_audio_driver(driver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}

int main() {
/*
    RtMidiOut *midi_out = 0;

    try {
        midi_out = new RtMidiOut();
    } catch(RtMidiError &error) {
        error.printMessage();
    }

    delete midi_out;
    */

    create_synth();

    while(1) {
        play_note(0, 60, 100);
        sleep(2);
        release_note(0, 60);
	    sleep(3);
    }

    return 0;
}