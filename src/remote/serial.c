#include "serial.h"

/*
 * Sensor -> Serial
 *
 * STEP 2: FROM Sensor processor to home (master) computer.
 * 1) Retrieve sensor data from named pipe (FIFO).
 * 2) Connect via Bluetooth to a IO buffer and write data to the file.
 */

void get_raw_data() {
	data_len = 0;

	mkfifo(SENSOR_DATA, 0666);
	sensor_fd = open(SENSOR_DATA, O_RDONLY);

	while(data_len > 0 ||  data_len != EOF ||
			(data_len == -1 && (errno == EAGAIN || errno == EINTR))) {
		data_len = read(sensor_fd, sensor_buf, BUF_SIZE);
		send_msg(sensor_buf, data_len);
	}
}

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
	//tcflush(blue_fd, TCIFLUSH);

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
		printf("Unable to open bluetooth communication port: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	fcntl(blue_fd, F_SETFL, 0); // modify blue_fd flags
}

/* Write data to serial output of bluetooth module */
void send_msg(char *msg, int len) {
	int res = write(blue_fd, msg, len);

	if(res < 0) {
		fputs("send_msg() failed to write.", stderr);
		exit(EXIT_FAILURE);
	}
}

int main() {
	open_port();
	configure();

	get_raw_data();
	return 0;
}
