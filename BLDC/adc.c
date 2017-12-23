#include "adc.h"
#include "bsp_tim.h"

void BLDC_StopAdc(void)
{
#if defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
	ADC_TRIG_TIM->CR1 &= ~TIM_CR1_CEN;
	ADC1->CR1 &= ~ADC_CR1_JEOSIE;
#else
	ADC_TRIG_TIM->CR1 &= ~TIM_CR1_CEN;
	DMA1->IFCR |= DMA_FLAG_TC1;
	DMA1_Channel1->CCR &= ~DMA_CCR_TCIE;
#endif
}

void BLDC_StartAdc(void)
{
#if defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
	ADC_TRIG_TIM->CR1 |= TIM_CR1_CEN;
	ADC1->CR1 |= ADC_CR1_JEOSIE;
	ADC1->CR2 |= ADC_CR2_JSWSTART;
#else
	ADC_TRIG_TIM->CR1 |= TIM_CR1_CEN;
	DMA_EnableTCIE();
#endif
}
