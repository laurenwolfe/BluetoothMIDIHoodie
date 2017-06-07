#ifndef BLUETOOTHMIDIHOODIE_HOST_SERIAL_H
#define BLUETOOTHMIDIHOODIE_HOST_SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control */
#include <errno.h>
#include <termios.h> /* POSIX terminal control */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#define BAUDRATE B9600
#define BLUE_TTY "tty.raspberrypi-SerialPort"
#define FIFO_PATH "/tmp/midi_fifo"
#define BUF_SIZE 255

void configure(int);
void open_port(void);
int init_modem(int);
void read_msg(int, char *, int);

int blue_fd, midi_fd, input_len;
char buf[BUF_SIZE];

#endif //BLUETOOTHMIDIHOODIE_HOST_SERIAL_H