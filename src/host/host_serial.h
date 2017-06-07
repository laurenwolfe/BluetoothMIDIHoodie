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
#include <stdbool.h>

#define BAUDRATE B9600
//#define BLUE_TTY "/dev/cu.raspberrypi-SerialPort"
#define BLUE_TTY "/dev/tty.BlueSMIRF-RNI-SPP"
#define MIDI_PATH "/tmp/midi_fifo"
#define BUF_SIZE 255
#define NUM_PINS 12


int blue_fd, midi_fd, input_len;
char sensor_buf[BUF_SIZE], midi_buf[BUF_SIZE];
bool pin_arr[NUM_PINS];

void configure(void);
void recv_bluetooth(void);
void translate_midi(int);
void convert_to_midi_hex(int);

#endif //BLUETOOTHMIDIHOODIE_HOST_SERIAL_H