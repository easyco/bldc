#ifndef __COMM_H
#define __COMM_H

#include "bsp_tim.h"

void BLDC_CommPWM(uint8_t phase);
void BLDC_StartAutoComm(void);
void BLDC_StopAutoComm(void);
void BLDC_UpdateCommPeriod(uint16_t period);

#endif
/* end of file */
