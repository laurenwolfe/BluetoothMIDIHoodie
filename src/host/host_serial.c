#include "host_serial.h"


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

	//Line-oriented / canonical input --requires a LF char (\n)
	options.c_lflag |= (ICANON | ECHO | ECHOE);

	//Canonical input
	options.c_oflag |= OPOST;

	//clean modem line and set attributes
	//tcflush(fd, TCIFLUSH);

	if(tcsetattr(blue_fd, TCSANOW, &options) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		tcsetattr(blue_fd, TCSANOW, &old_options);
		exit(EXIT_FAILURE);
	}
}

/* Open serial port */
void open_port() {
	blue_fd = open(BLUE_TTY, O_RDWR | O_NOCTTY | O_NDELAY);

	if(blue_fd == -1) {
		perror("Unable to open bluetooth communication port - \n");
		exit(EXIT_FAILURE);
	}

	fcntl(blue_fd, F_SETFL, 0); // modify fd flags
}

int main() {
	open_port();

	if (input_len == read(blue_fd, buf, 255) < 0) {
		perror("Bluetooth read failure.\n");
		exit(EXIT_FAILURE);
	}

	mkfifo(FIFO_PATH, 0666);

	buf[0] = 0x9e;
	buf[1] = 0x8e;
	buf[2] = 0x90;
	buf[3] = 0x80;
	buf[4] = 0x91;
	buf[5] = 0x81;
	buf[6] = 0x96;
	buf[7] = 0x86;

	while(1) {
		midi_fd = open(FIFO_PATH, O_WRONLY);
		write(midi_fd, buf, BUF_SIZE);
		close(midi_fd);
		if(input_len < BUF_SIZE) {
			input_len = 0;
		}
	}

	return 0;
}