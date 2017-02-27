#include "Communications.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


unsigned char serial_write_data(char *data, int len)
{
	unsigned char ret;

	ret = Uart_Write(data);

	return ret;
}

unsigned char serial_read_data(char *buf, int len) {

	unsigned char ret;

	ret = Uart_Read(buf, len);

	return ret;
}

unsigned char serial_blocking_read_next(void *buf) {

	unsigned char ret;

    ret = Uart_Read(buf, 1);

    return ret;
}

unsigned char serial_write_all(char *src, int len) {

	unsigned char ret;

	ret = serial_read_data(src, len);

	return ret;
}

unsigned char serial_read_all(char *dst, int len) {

	unsigned char ret;

	ret = serial_read_data(dst, len);

	return ret;
}

int serial_read_line(char *buf, int len) {

	int i;
	bool found = false;

	for (i = 0; i < len - 1; i++) {
		serial_blocking_read_next(&buf[i]);

		if (buf[i] != '\n')
			found = true;
		else if (found)
			break;
	}

	if (!found || i == len - 1)
		return 0;

	return (i + 1);
}

int serial_read_integer(int *val) {

	unsigned int i;
	char buf[1024], *ptr = NULL, *end;
	unsigned char ret;
	int value;


	do {
		ret = serial_read_line(buf, sizeof(buf));

		for (i = 0; i < (unsigned int) ret; i++) {
			if (buf[i] != '\n') {
				if (!ptr)
					ptr = &buf[i];
			} else if (!!ptr) {
				break;
			}
		}
	} while (!ptr);

	buf[i] = '\0';

	value = (int) strtol(ptr, &end, 10);
	if (ptr == end)
		return 1;

	*val = value;
	return 0;
}

int execute_command(char *cmd) {

	int resp;

	Uart_Mux(LibIIO_MUX);

	serial_write_data(cmd, strlen(cmd));

	serial_read_integer(&resp);

	return resp;
}
