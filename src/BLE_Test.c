/*
 * BLE_Test - Main of BLE_Test
 *
 * Author: Jake Greaves
 */

#include "BLE_Module.h"
#include "common.h"
#include "Communications.h"
#include "Wireless_Sense_Module.h"
#include <services/pwr/adi_pwr.h>
#include <services/gpio/adi_gpio.h>
#include <sys/platform.h>
#include "adi_initialize.h"
#include "system_ADuCM3029.h"
#include "LibIIO_Interface.h"
#include "ADuCM3029_Sleep_Interface.h"

#define BLE_BINARY (unsigned char*) sps_device_580_bin

static uint8_t GPIOCallbackMem[ADI_GPIO_MEMORY_SIZE];

//Rudimentary delay in milliseconds
void Delay_ms(unsigned int mSec)
{
	int d1, d2;
	d1 = mSec;
	d2 = mSec;

	for ( d1 = 0; d1 < mSec; d1++ ) {
		for ( d2 = 0; d2 < 3400; d2++ ) {
			asm(""); //this is needed to avoid compiler optimization removing this delay
		}
	}
}


//Init micro specific components
unsigned char Micro_Init(void) {

	//set pins
	adi_initComponents();

	//test system initialization
	test_Init();

	//Init power services
	if (adi_pwr_Init() != ADI_PWR_SUCCESS) {
		return 1;
	}

	//set clock divider (1)
	if (ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1)) {
		return 1;
	}

	//set clock divider (1)
	if (ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1)) {
		return 1;
	}

	//Init GPIO
	if (ADI_GPIO_SUCCESS != adi_gpio_Init(GPIOCallbackMem, ADI_GPIO_MEMORY_SIZE)) {
		return 1;
	}

	return 0;
}


int main(void)
{
	char BLE_TxPayload[255]; //Tx buffer
	//char BLE_RxBuffer[255];	//Rx buffer

	//Init necessary micro components
	if(Micro_Init()!= 0)
		return 1;

	if(Ble_Spi_Boot(BLE_BINARY, IMAGE_SIZE) != 0)
	  return 1;

	//initialize UART
	Uart_Init(); //Init serial comms

	//ensure SENSE is on
	Sense_Activate();

	while(1)
	{
		//check if BLE is asleep and if glue should also sleep
		if(Ble_Sleep)
			EnterSleep();

		//getTempVals(BLE_TxPayload);
		//Read Temp-Module sensor value
		LibIIO_ReadBuf(BLE_TxPayload);

		//Write sensor value to BLE
		Ble_Uart_Write(BLE_TxPayload);

		Delay_ms(10);
	}
}
