/*
 * LibIIO_Interface.h
 *
 *  Created on: 25 Feb 2017
 *      Author: jtgre
 */

#ifndef LIBIIO_INTERFACE_H_
#define LIBIIO_INTERFACE_H_

#include <services/gpio/adi_gpio.h>

#define SENSE1000_RESET_PORT 	ADI_GPIO_PORT1
#define SENSE1000_RESET_PIN 	ADI_GPIO_PIN_2

#define SENSE1000_RESET_LENGTH 	15000


extern void Delay_ms(unsigned int mSec);


//Function prototypes
unsigned char LibIIO_Open(void);

unsigned char LibIIO_ReadBuf(char Buf[]);

unsigned char Sense_Wait_For_Boot(void);

unsigned char Sense_Activate(void);

unsigned char Sense_Deactivate(void);
#endif /* LIBIIO_INTERFACE_H_ */
