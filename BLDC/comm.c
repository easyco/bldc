#include "comm.h"
#include "bldc.h"

#ifndef USE_SYNC_DRIVER
/* 非同步整流 */
const uint16_t PwmCommTable[6] =
{
	(uint16_t)(TIM_CCER_CC1E | TIM_CCER_CC2NE), // AB
	(uint16_t)(TIM_CCER_CC1E | TIM_CCER_CC3NE), // AC
	(uint16_t)(TIM_CCER_CC2E | TIM_CCER_CC3NE),	// BC
	(uint16_t)(TIM_CCER_CC2E | TIM_CCER_CC1NE),	// BA
	(uint16_t)(TIM_CCER_CC3E | TIM_CCER_CC1NE),	// CA
	(uint16_t)(TIM_CCER_CC3E | TIM_CCER_CC2NE)	// CB
};
#else
/* 同步整流 */
const uint16_t PwmCommTable[6] = {
	(uint16_t)(TIM_CCER_CC1E | TIM_CCER_CC1NE | TIM_CCER_CC2NE), // AB
	(uint16_t)(TIM_CCER_CC1E | TIM_CCER_CC1NE | TIM_CCER_CC3NE), // AC
	(uint16_t)(TIM_CCER_CC2E | TIM_CCER_CC2NE | TIM_CCER_CC3NE), // BC
	(uint16_t)(TIM_CCER_CC2E | TIM_CCER_CC2NE | TIM_CCER_CC1NE), // BA
	(uint16_t)(TIM_CCER_CC3E | TIM_CCER_CC3NE | TIM_CCER_CC1NE), // CA
	(uint16_t)(TIM_CCER_CC3E | TIM_CCER_CC3NE | TIM_CCER_CC2NE)	 // CB
};
#endif

void BLDC_CommPWM(uint8_t phase)
{
	PWM_TIM->CNT = 0;
	PWM_TIM->CCER = PwmCommTable[phase] | TIM_CCER_CC4E;
}

void BLDC_StartAutoComm(void)
{
	COMM_TIM->CNT = 0;
	COMM_TIM->CR1 |= TIM_CR1_CEN;			// 计数器使能
	COMM_TIM->DIER |= TIM_DIER_UIE;
}

void BLDC_StopAutoComm(void)
{
	COMM_TIM->SR &= ~TIM_FLAG_UPDATE;
	COMM_TIM->CR1 &= ~TIM_CR1_CEN;			// 计数器停止
	COMM_TIM->DIER &= ~TIM_DIER_UIE;
}

void BLDC_UpdateCommPeriod(uint16_t period)
{
	COMM_TIM->ARR = period;
}
/* end of file */
