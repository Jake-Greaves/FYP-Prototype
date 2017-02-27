/*
 * LibIIO_Utility.h
 *
 *  Created on: 25 Feb 2017
 *      Author: jtgre
 */

#ifndef LIBIIO_UTILITY_H_
#define LIBIIO_UTILITY_H_

//Function prototypes

int execute_command(char *cmd);

unsigned char serial_read_integer(int *val);

unsigned char serial_write_all(char *src, int len);

unsigned char serial_read_all(char *dst, int len);

unsigned char serial_write_data(char *data, int len);

unsigned char serial_read_data(char *buf, int len);

unsigned char serial_blocking_read_next(char *buf);

int serial_read_line(char *buf, int len);


#endif /* LIBIIO_UTILITY_H_ */
