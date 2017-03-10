/*
 * LibIIO_Interface.c - Temp-Module main functions
 *
 * Author: Jake Greaves
 */

#include "Communications.h"
#include "LibIIO_Utility.h"
#include "LibIIO_Interface.h"
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

unsigned char LibIIO_ReadBuf(char *retBuf) {

	char buf[1024];
	int ret;
	char *pos;
	int offset;

	snprintf(buf, sizeof(buf), "READBUF %s %lu\r\n",
									"iio:device0", (unsigned long) 64);

	ret = execute_command(buf);

	ret = serial_read_all((void*)buf, 39);
	pos = strstr(buf, "\n");

	offset = buf - pos; //TODO: dynamically filter out mask

	strcpy(retBuf, buf + 9);

	return ret;
}

unsigned char Sense_Wait_For_Boot(void)
{
	Delay_ms(SENSE1000_RESET_LENGTH);

	return 0;
}

unsigned char Sense_Activate(void)
{
	//Ensure reset pin is high
	adi_gpio_SetHigh(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN);
	adi_gpio_OutputEnable(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN, true);

	//wait for calibration
	Sense_Wait_For_Boot();

	//Open device
	LibIIO_Open();

	return 0;
}

unsigned char Sense_Deactivate(void)
{
	adi_gpio_SetLow(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN);
	adi_gpio_OutputEnable(SENSE1000_RESET_PORT, SENSE1000_RESET_PIN, true);

	return 0;
}
