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

void process_input() {
	int pin, status;

	for(int i = 0; i < n_read; i++) {
		status = buf[i] & 0xF0;
		pin = (buf[i] & 0x15) + 0x60;

		printf("status: %x, pin: %x\n", status, pin);

		if(status == 0x90) {
			play_note(0, pin, 100);
		} else {
			release_note(0, pin);
		}
	}
}

int main() {
    create_synth();

	int ret = mkfifo(FIFO_PATH, 0666);
	n_read = 0;
	delay = 0;

	//Wait 30 minutes until severing the connection
	while(delay < 1800) {
		if(ret < 0 && errno != EACCES) {
			printf("Irrecoverable file I/O error: %s\n", strerror(errno));
			return -1;
		}

		midi_fd = open(FIFO_PATH, O_RDONLY);
		n_read = read(midi_fd, buf, n_read);
		close(midi_fd);

		//Process the chars
		if(n_read > 0) {
			process_input();
			delay = 0;
		} else {
			sleep(1);
			delay++;
		}

		//Create offset if buffer isn't read
		if(n_read < BUF_SIZE || n_read < 0) {
			n_read = 0;
		}
    }

    return 0;
}