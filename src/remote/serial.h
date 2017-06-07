#ifndef SERIAL_H

#define SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control */ 
#include <errno.h>
#include <termios.h> /* POSIX terminal control */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#define BAUDRATE B9600
#define BLUE_TTY "/dev/ttyO4"
#define SENSOR_DATA "~/tmp/midi"
#define BUF_SIZE 255
#define DEBUG true

int blue_fd, sensor_fd, data_len;
char sensor_buf[BUF_SIZE];

void get_raw_data(void);
void configure(void);
void open_port(void);
void send_msg(char *, int);

#endif /** SERIAL_H */
