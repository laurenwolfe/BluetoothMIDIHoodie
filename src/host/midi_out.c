#include "midi_out.h"

/*
 * Host Serial Bluetooth -> Virtual Midi Controller
 *
 * STEP 4: CONVERT hexadecimal midi values into audio using FluidSynth C library.
 * 1) Get midi data from named pipe/FIFO.
 * 2) Create Synth/Midi Player objects and process/output the data to external speakers.
 *
 */

void create_synth() {
    settings = new_fluid_settings();
    fluid_settings_setstr(settings, "audio.driver", "coreaudio");

    synth = new_fluid_synth(settings);
    driver = new_fluid_audio_driver(settings, synth);

	if(!settings || !synth || !driver) {
		printf("Midi interface failed to initialize.");
	}

	//Specify a soundfont file (determines the tonal quality of the audio output)
    fluid_synth_sfload(synth, SOUNDFONT, 1);
}

// Add 0x60 to make notes in middle range of midi tones,
// mask values to separate the status code and note bits.
// Values:
// - 0x90 == note sounding, 0x80 == note ended
// - 0x0-0x11 represent notes along the Middle C scale (approximately).
void process_input(int n_read) {
	for(int i = 0; i < n_read; i++) {
		int note = (buf[i] & 0x15) + SCALE;
		int status = buf[i] & CMD_MASK;

		if(status == 0x90)
			fluid_synth_noteon(synth, CHANNEL, note, VELOCITY);
		else
			fluid_synth_noteoff(synth, CHANNEL, note);
	}
}

void delete_synth() {
	delete_fluid_audio_driver(driver);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);
}

int main() {
    create_synth();

	mkfifo(MIDI_PATH, 0666);
	midi_fd = open(MIDI_PATH, O_RDONLY);

	while((n_read = read(midi_fd, buf, BUF_SIZE)) != EOF) {
		if(n_read == -1 && (errno == EAGAIN || errno == EINTR)) return -1;

		process_input(n_read);
	}

	close(midi_fd);
	return 0;
}