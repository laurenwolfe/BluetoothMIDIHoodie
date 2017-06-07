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
	struct termios old_options, options;

	// Retrieve options
	if(tcgetattr(blue_fd, &options) < 0) {
		printf("Error from tcgetattr: %s \n", strerror(errno));
		exit(EXIT_FAILURE);
	}

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
		exit(EXIT_FAILURE);
	}
}

/* Open serial port */
void recv_bluetooth() {
	if((blue_fd = open(BLUE_TTY, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
		printf("Unable to open bluetooth communication port: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	fcntl(blue_fd, F_SETFL, 0); // modify fd flags
}

void translate_midi(int arr_len) {
	if(arr_len <= 0)
		return;

	// Retrieve data from controller
	mkfifo(MIDI_PATH, 0666);

	if((midi_fd = open(MIDI_PATH, O_WRONLY)) <= 1)
		return;

	//convert hex values into to midi code
	// add to audio output buffer (FIFO named pipe)
	for(int i = 0; i < arr_len; i++) {
		convert_to_midi_hex(i);
	}

	write(midi_fd, pin_arr, NUM_PINS);
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

int main() {
	//sample midi data
	sensor_buf[0] = 0x9e;
	sensor_buf[1] = 0x8e;
	sensor_buf[2] = 0x90;
	sensor_buf[3] = 0x80;
	sensor_buf[4] = 0x91;
	sensor_buf[5] = 0x81;
	sensor_buf[6] = 0x96;
	sensor_buf[7] = 0x86;

	//Set up bluetooth serial connection and receive sensor data
	configure();
	recv_bluetooth();

	//Inject sensor data from sensor_buffer and convert into MIDI format
	while((input_len = read(blue_fd, sensor_buf, BUF_SIZE)) != EOF) {

		if(input_len > 0 || (input_len == -1 && !(errno == EAGAIN && errno == EINTR))) {
			translate_midi(input_len);
		}
	}
	return 0;
}