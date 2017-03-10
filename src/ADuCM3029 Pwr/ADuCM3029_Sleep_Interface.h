/*
 * ADuCM3029_Sleep_Interface.h - Reduced power sleep main functions
 *
 * Author: Jake Greaves
 */

#ifndef _SLEEP_INTERFACE_H_
#define _SLEEP_INTERFACE_H_

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include <services/gpio/adi_gpio.h>
#include <services/pwr/adi_pwr.h>
#include "stdio.h"

/******************************************************************************/
/* External variables                                                         */
/******************************************************************************/
extern bool_t Ble_Sleep; //global flag for if BLE is currently sleeping (extended sleep)

/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

//External Interupt callback. Should be externally ties to the BLE CTS pin
void ExIRQCallback(void *pCBParam, uint32_t Event, void *pArg);

//Sleep mode. Also handles wake up event
char EnterSleep(void);

#endif
