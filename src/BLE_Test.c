

#include "BLE_Module.h"
#include "common.h"
#include "Communications.h"
#include "sps_device_580_FINAL.h"
#include "Placeholder_TempInterface.h"
#include <services/pwr/adi_pwr.h>
#include <services/gpio/adi_gpio.h>
#include <sys/platform.h>
#include "adi_initialize.h"
#include "system_ADuCM3029.h"
#include "LibIIO_Interface.h"

#define BLE_BINARY (unsigned char*) sps_device_580_Final_bin

#undef JLINK_DEBUGGING


char BLE_RxBuffer[255];


static uint8_t GPIOCallbackMem[ADI_GPIO_MEMORY_SIZE];
char BLE_Payload[255];


void Delay_ms(unsigned int mSec)
{
  int d1, d2;
  d1 = mSec;
  d2 = mSec;
  for ( d1 = 0; d1 < mSec; d1++ )
  {
   for ( d2 = 0; d2 < 3400; d2++ )
   {
	   asm("");
   }
  }
}


unsigned char Micro_Init(void) {

	//set pins
	adi_initComponents();

	/* test system initialization */
	test_Init();

	if (adi_pwr_Init() != ADI_PWR_SUCCESS) {
		return 1;
	}

	if (ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1)) {
		return 1;
	}

	if (ADI_PWR_SUCCESS != adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1)) {
		return 1;
	}

	if (adi_gpio_Init(GPIOCallbackMem, ADI_GPIO_MEMORY_SIZE)
			!= ADI_GPIO_SUCCESS) {
		return 1;
	}

	return 0;
}


void main(void)
{
  Micro_Init();

  adi_gpio_OutputEnable(ADI_GPIO_PORT1, ADI_GPIO_PIN_12, true);
  	adi_gpio_SetHigh(ADI_GPIO_PORT1, ADI_GPIO_PIN_12);

#ifdef JLINK_DEBUGGING
  if(err)
  {
    DEBUG_MESSAGE("Micro failed to boot\n");
  }
#endif

  if(Ble_Spi_Boot(BLE_BINARY, IMAGE_SIZE) != 0)
  {
#ifdef JLINK_DEBUGGING
    DEBUG_MESSAGE("Dialog14580 failed to boot\n");
#endif
  }

  Uart_Init(); //TODO: change mux pin

  LibIIO_Open();

  while(1)
  {
	char BLE_TxPayload[1024];

	LibIIO_ReadBuf(BLE_TxPayload);
    //getTempVals(BLE_TxPayload); //Test for BLE communications. RNG
    Ble_Uart_Write(BLE_TxPayload);

#ifdef JLINK_DEBUGGING
    DEBUG_MESSAGE(BLE_TxPayload);
#endif

    Delay_ms(10);
  }
}
