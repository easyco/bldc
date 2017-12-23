#ifndef __PWM_H
#define __PWM_H

#include "bsp_tim.h"

void BLDC_StartPWM(void);
void BLDC_StopPWM(void);
void BLDC_UpdatePwmDuty(uint16_t duty);

#endif
/* end fo fiel */
