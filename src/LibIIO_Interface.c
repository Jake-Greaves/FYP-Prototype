#include "Communications.h"
#include "LibIIO_Utility.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned char LibIIO_Open(void) {

	char buf[1024] = "";
	char *ptr = "";
	int ret;
	int i = 0;

	snprintf(&buf[0], (size_t)sizeof(buf), "OPEN %s %lu ",
				"iio:device0", (unsigned long) 1);

	ptr = buf + strlen(buf);

	for (i = 1; i > 0; i--, ptr += 8) {

		snprintf(ptr, (ptr - buf) + i * 8, "%08lu", (unsigned long)0x03);
	}

	strcpy(ptr,"\r\n");

	ret = execute_command(buf);

	return ret;
}

unsigned char LibIIO_ReadBuf(char retBuf) {

	char buf[1024];
	int ret;

	snprintf(buf, sizeof(buf), "READBUF %s %lu\r\n",
									"iio:device0", (unsigned long) 64);


	ret = execute_command(buf);

	ret = serial_read_all((void*)buf, 39);

	retBuf = *buf;
	return ret;
}
