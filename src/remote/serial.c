#include "serial.h"


/* Configure terminal mode */
void configure(int fd) {
	struct termios old_options, options;
	
	// Retrieve options
	if(tcgetattr(fd, &options) < 0) {
		printf("Error from tcgetattr: %s \n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	tcgetattr(fd, &old_options);

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

	if(tcsetattr(fd, TCSANOW, &options) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		tcsetattr(fd, TCSANOW, &old_options);
		exit(EXIT_FAILURE);
	}
} 


/* Open serial port */
int open_port() {
	int fd; 

	fd = open(BLUE_TTY, O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd == -1) {
		perror("Unable to open bluetooth communication port - ");
		exit(EXIT_FAILURE);
	}
	
	fcntl(fd, F_SETFL, 0); // modify fd flags

	return fd;
}


/* Write data to serial output of bluetooth module */
void send_msg(int fd, char *msg, int len) {
	if(write(fd, msg, len) < 0) {
		fputs("send_msg() failed to write.", stderr);
		exit(EXIT_FAILURE);
	}
}


int main() {
	//char *msg = "Testing!\n";

	int fd = open_port();

	configure(fd);

	send_msg(fd, "", 9);
	close(fd);

	return 0;
}
