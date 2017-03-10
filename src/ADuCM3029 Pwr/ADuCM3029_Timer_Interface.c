/*
 * ADuCM3029_Timer_Interface.c - Timer functions to detect BLE timeout
 *
 * Author: Jake Greaves
 */

#include <stdio.h>
#include <services/tmr/adi_tmr.h>
#include "ADuCM3029_Timer_Interface.h"
#include <services/gpio/adi_gpio.h>
#include <services/pwr/adi_pwr.h>


static uint8_t aDeviceMemory0[ADI_TMR_MEMORY_SIZE];

//Global BLE sleep flag
bool_t Ble_Sleep = false;

ADI_TMR_HANDLE hDevice; //Timer handle


//begin timer count
char Timer_Count(uint32_t nDeviceNum)
{
    ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
    
    result = adi_tmr_Open(nDeviceNum,aDeviceMemory0,ADI_TMR_MEMORY_SIZE,&hDevice);
    
    if (ADI_TMR_SUCCESS == result)
        result = adi_tmr_RegisterCallback( hDevice, GPTimer0Callback ,hDevice);

    if (ADI_TMR_SUCCESS == result)
        result = adi_tmr_SetPrescaler(hDevice, ADI_GPT_PRESCALER_256);

    if (ADI_TMR_SUCCESS == result)
        result = adi_tmr_SetClockSource(hDevice, ADI_TMR_CLOCK_HFOSC);

    if (ADI_TMR_SUCCESS == result)
        result = adi_tmr_SetRunMode( hDevice, ADI_TMR_FREE_RUNNING_MODE);

    if (ADI_TMR_SUCCESS == result)
        result = adi_tmr_SetCountMode(hDevice, ADI_TMR_COUNT_DOWN);

    if (ADI_TMR_SUCCESS == result)
        adi_tmr_Enable(hDevice, true); 

    if (ADI_TMR_SUCCESS == result)
      return 0;
    
    else
      return 1;
}

//close timer to avoid further interrupts
char Close_Timer(void)
{
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  if (ADI_TMR_SUCCESS == result)
      result = adi_tmr_Enable(hDevice,false);
  
  if (ADI_TMR_SUCCESS == result)
      result = adi_tmr_Close(hDevice);
  
  if (ADI_TMR_SUCCESS == result)
    return 0;
  
  else
    return 1;
}

//called when timer times out instead of being closed. Indicates BLE is no longer available
char Handle_Timeout(void)
{
  ADI_TMR_RESULT tmrResult = ADI_TMR_SUCCESS;
  
  if (ADI_TMR_SUCCESS == tmrResult)
      tmrResult = Close_Timer();
  
  if (ADI_TMR_SUCCESS == tmrResult)
    return 0;
  
  else
    return 1;
}

//Timer callback
void GPTimer0Callback(void *pCBParam, uint32_t Event, void *pArg)
{
  switch(Event)
  {
      case ADI_TMR_EVENT_TIMEOUT:
        Ble_Sleep = true; //alert system that BLE is no longer available
        Handle_Timeout(); //handle timeout
        break;
        
     default:
        break;
  }
}

