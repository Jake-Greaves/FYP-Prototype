/*
 * ADuCM3029_Sleep_Interface.c - Reduced power sleep main functions
 *
 * Author: Jake Greaves
 */

#include "ADuCM3029_Sleep_Interface.h"
#include <services/gpio/adi_gpio.h>
#include <services/pwr/adi_pwr.h>
#include "Communications.h"
#include "stdio.h"
#include "LibIIO_Interface.h"

IRQn_Type eIrq = XINT_EVT1_IRQn;

volatile bool_t bHibernateExitFlag = 0; //when set indicates that the glue should leave sleep mode. Set within wake interrupt


//wake callback
void ExIRQCallback(void *pCBParam, uint32_t Event, void *pArg)
{
  adi_gpio_RegisterCallback(eIrq, NULL, NULL);
  adi_pwr_ExitLowPowerMode(&bHibernateExitFlag);
}

//enter hibernate sleep. On wake, resumes within this function as well
char EnterSleep(void)
{
  ADI_PWR_RESULT pwrResult = ADI_PWR_SUCCESS;
  ADI_GPIO_RESULT gpioResult = ADI_GPIO_SUCCESS;
  
  //close UART
  Uart_Close();
  
  //set XINT priority high to override current timer interrupt. 0 is default and highest priority
  //NVIC_SetPriority(eIrq, 0);
  
  //revert WAKE0 (P0.15) mux setting to GPIO mode
  //pADI_GPIO0->CFG &= ~(3<<30);  // clear config bits 31:30 to zero
       
  //register wake interrupt
  if (ADI_GPIO_SUCCESS == gpioResult)
    gpioResult = adi_gpio_RegisterCallback(eIrq, ExIRQCallback, NULL);
  
  adi_gpio_InputEnable(ADI_GPIO_PORT1, ADI_GPIO_PIN_0, true);
    
  //Enable external interrupt to wake device
  if (ADI_GPIO_SUCCESS == gpioResult)
    gpioResult = adi_gpio_EnableExIRQ(eIrq, ADI_GPIO_IRQ_FALLING_EDGE);

  //'Shutdown' Sense1000
  Sense_Deactivate();

  //Timeout has occured, Enter Hibernation until interrupt sets flag //TODO: only works in Flexi. Works in hibernation in IAR
  if (ADI_GPIO_SUCCESS == gpioResult)
   pwrResult = adi_pwr_EnterLowPowerMode(ADI_PWR_MODE_FLEXI, &bHibernateExitFlag, 0);

  //disable ext interrupt
  if (ADI_PWR_SUCCESS == pwrResult)
    gpioResult = adi_gpio_DisableExIRQ(eIrq);

  //unregister callback
  if (ADI_GPIO_SUCCESS == gpioResult)
    gpioResult = adi_gpio_RegisterCallback(eIrq, NULL, NULL);
  
  if (ADI_GPIO_SUCCESS == gpioResult)
    gpioResult = adi_gpio_InputEnable(ADI_GPIO_PORT1, ADI_GPIO_PIN_0, false);
  
  //Ble is awake
  Ble_Sleep = false;
  
  //Reinit UART
  Uart_Init();
  
  //'Wake' Sense1000
  Sense_Activate();

  if ((pwrResult == ADI_PWR_SUCCESS) && (gpioResult == ADI_GPIO_SUCCESS))
    return 0;
  
  else
    return 1;
}
