#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdbool.h>
#include "stm32f1xx_hal.h"

//#define SYSCLK_SOURCE_HSE

#ifdef SYSCLK_SOURCE_HSE
#define SYSCLK				72000000
#else
#define SYSCLK_SOURCE_HSI
#define SYSCLK				64000000
#endif

void SystemClock_Config(void);
void Error_Handler(void);

#endif
/* end of file */
