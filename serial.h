#ifndef SERIAL_H

#define SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control */ 
#include <errno.h>
#include <termios.h> /* POSIX terminal control */

#define BAUDRATE B9600
#define BLUE_TTY "/dev/ttyO4"

void configure(int);
int open_port(void);
int init_modem(int);
void send_msg(int, char *, int);

#endif /** SERIAL_H */
