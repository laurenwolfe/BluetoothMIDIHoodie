#include "host_serial.h"

/*
 * Serial -> Host Serial Bluetooth
 *
 * STEP 3: DOWNLOAD from embedded hardware to the home computer via Bluetooth.
 * 1) Initialize BT connection and wait for serial to write sensor data.
 * 2) Convert sensor data to Midi format.
 * 3) Send converted data to virtual midi controller.
 */

/* Configure terminal mode */
void configure() {
	blue_fd = open(BLUE_TTY, O_RDONLY | O_NOCTTY | O_NDELAY);

	struct termios old_options, options;

	// Retrieve options
//	if(tcgetattr(blue_fd, &options) < 0) {
//		printf("Error from tcgetattr: %s \n", strerror(errno));
//		exit(EXIT_FAILURE);
//	}

	tcgetattr(blue_fd, &old_options);

	// Configure baudrate
	cfsetospeed(&options, BAUDRATE);
	cfsetispeed(&options, BAUDRATE);

	// Ignore modem controls
	options.c_cflag |= (CLOCAL | CREAD);

	// 8N1 (8-bit, no parity, one stop bit)
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8; // 8-bit chars
	options.c_cflag &= ~PARENB; // no parity
	options.c_cflag &= ~CSTOPB; // 1 stop bit
	options.c_cflag &= ~CRTSCTS; //hw flow ctl

	//raw input
	options.c_lflag &= (ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;

	//clean modem line and set attributes
	//tcflush(fd, TCIFLUSH);

	if(tcsetattr(blue_fd, TCSANOW, &options) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		tcsetattr(blue_fd, TCSANOW, &old_options);
		//exit(EXIT_FAILURE);
	}
	printf("Success!!!\n");

}

/* Open serial port */
void recv_bluetooth() {
	int i = 1;
	blue_fd = open(BLUE_TTY, O_RDONLY | O_NOCTTY | O_NDELAY);

	while(i < 50 && blue_fd < 0) {
		blue_fd = open(BLUE_TTY, O_RDONLY | O_NOCTTY | O_NDELAY);
		printf("Unable to open bluetooth resource: %s,\n Retry # %d\n",
		       strerror(errno), i);
		i++;
		sleep(2);
	}
	printf("Success!!!\n");
}

void translate_midi(int arr_len) {
	int midi_res;

	if(DEBUG) {
		/*
		mkfifo(MIDI_PATH, 0666);

		if((midi_fd = open(MIDI_PATH, O_WRONLY | O_NOCTTY | O_NDELAY)) < 0) {
			printf("Can't open midi fifo: %s\n", strerror(errno));
		}

		midi_res = write(midi_fd, midi_buf, 9);

		if(midi_res < 0) {
			printf("Can't write to midi fifo: %s\n", strerror(errno));
		} else {
			printf("success!!\n");
		}
		 */
	}
	/*
	if(arr_len <= 0) {
		printf("isset array is empty.\n");
		return;
	}

	// Retrieve data from controller
	mkfifo(MIDI_PATH, 0666);

	if((midi_fd = open(MIDI_PATH, O_WRONLY | O_NOCTTY | O_NDELAY)) <= 1) {
		printf("Can't open midi fifo: %s\n", strerror(errno));
		return;
	}

	//convert hex values into to midi code
	// add to audio output buffer (FIFO named pipe)
	for(int i = 0; i < arr_len; i++) {
		convert_to_midi_hex(i);
	}

	write(midi_fd, midi_buf, arr_len);
	 */
}

//converts raw input to midi command value.
//for future encoding purposes
void convert_to_midi_hex(int i) {
	if(pin_arr[i])
		midi_buf[i] |= 0x80;     //deactivate note.
	else
		midi_buf[i] |= 0x90; 	//sound note.

	//reset isset array representation.
	pin_arr[i] = !pin_arr[i];
}

// read into sensor_buf, store current state in in pin_arr,
// midi_buf for translated midi data
int main() {
	//int j = 0;
	sleep(3);
	printf("Launching in host_serial now!\n");
	//sample midi data
	midi_buf[0] = 0x9e;
	midi_buf[1] = 0x8e;
	midi_buf[2] = 0x90;
	midi_buf[3] = 0x80;
	midi_buf[4] = 0x91;
	midi_buf[5] = 0x81;
	midi_buf[6] = 0x96;
	midi_buf[7] = 0x86;
	midi_buf[8] = '\n';

	//Set up bluetooth serial connection and receive sensor data
	configure();
	//recv_bluetooth();
	//printf("Can't open midi fifo: %s\n", strerror(errno));


	//Inject sensor data from sensor_buffer and convert into MIDI format
	//while(j < 50) {
		//input_len = read(blue_fd, sensor_buf, BUF_SIZE);

		if(DEBUG) {
			translate_midi(input_len);
		} else {
			/*
			if(input_len > 0) {
				// Write
				translate_midi(input_len);
				printf("Translated midi!\n");
				j = 0;
			} else {
				printf("Can't read midi fifo: %s\n", strerror(errno));
				j++;
				sleep(2);
			}
			 */
		}

	//}

	printf("Bottom of host_ser while loop!\n");
	close(blue_fd);
	return 0;
}