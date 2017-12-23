#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "stm32f1xx_hal.h"
#include "bsp_adc.h"

#define PWM_TIM						TIM1
#define PWM_CLK_Enable()			__HAL_RCC_TIM1_CLK_ENABLE()

#define PWM_FREQ					20000
#define PWM_TIM_PRESCALER			0
#define PWM_TIM_PERIOD				(SystemCoreClock / PWM_FREQ - 1)
#define PWM_DEADTIME				5

#define COMM_TIM					TIM2
#define COMM_TIM_CLK_Enable()		__HAL_RCC_TIM2_CLK_ENABLE()
#define COMM_TIM_FREQ				1000000
#define COMM_TIM_PRESCALER			(SystemCoreClock / COMM_TIM_FREQ - 1)
#define COMM_TIM_PERIOD				0xFFFF
#define COMM_TIM_IRQn				TIM2_IRQn
#define COMM_TIM_IRQHandler			TIM2_IRQHandler

#if defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
#define ZCNT_TIM					TIM3
#define ZCNT_TIM_CLK_Enable()		__HAL_RCC_TIM3_CLK_ENABLE()

#define ADC_TRIG_TIM				TIM4
#define ADC_TRIG_TIM_CLK_Enable()	__HAL_RCC_TIM4_CLK_ENABLE()
#define ADC_TRIG_TIM_IRQn			TIM4_IRQn
#define ADC_TRIG_TIM_IRQHandler		TIM4_IRQHandler
#else
#define ZCNT_TIM					TIM4
#define ZCNT_TIM_CLK_Enable()		__HAL_RCC_TIM4_CLK_ENABLE()

#define ADC_TRIG_TIM				TIM3
#define ADC_TRIG_TIM_CLK_Enable()	__HAL_RCC_TIM3_CLK_ENABLE()
#define ADC_TRIG_TIM_IRQn			TIM3_IRQn
#define ADC_TRIG_TIM_IRQHandler		TIM3_IRQHandler
#endif

#define ZCNT_TIM_FREQ				1000000
#define ZCNT_TIM_PRESCALER			(SystemCoreClock / ZCNT_TIM_FREQ - 1)
#define ZCNT_TIM_PERIOD				0xFFFF

#define ADC_TRIG_TIM_PRESCALER		0
#define ADC_TRIG_PERIOD				10	// uS
#define ADC_TRIG_TIM_PERIOD			(10 * (SystemCoreClock / 1000000))


void TIM_InitPwm(void);
void TIM_InitComm(void);
void TIM_InitCrossCnt(void);
void TIM_InitAdcTrigger(void);

#endif

