#include "pwm.h"

void BLDC_StartPWM(void)
{
	PWM_TIM->CR1 |= TIM_CR1_CEN;	// 计数器使能
	PWM_TIM->BDTR |= TIM_BDTR_MOE;	// 开启OC和OCN输出
}

void BLDC_StopPWM(void)
{
	PWM_TIM->CR1 &= ~TIM_CR1_CEN;
	PWM_TIM->BDTR &= ~TIM_BDTR_MOE;
}

void BLDC_UpdatePwmDuty(uint16_t duty)
{
	PWM_TIM->CCR1 = duty;
	PWM_TIM->CCR2 = duty;
	PWM_TIM->CCR3 = duty;
	PWM_TIM->CCR4 = duty;
}
/* end of file */
