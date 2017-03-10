#ifndef _TIMER_INTERFACE_H_
#define _TIMER_INTERFACE_H_

#include <services/tmr/adi_tmr.h>

#define TIMER_DEVICE_0 0


char Timer_Count(uint32_t nDeviceNumber);
char Handle_Timeout(void);
void GPTimer0Callback(void *pCBParam, uint32_t Event, void *pArg);
char Close_Timer(void);
#endif //_TIMER_INTERFACE_H_
