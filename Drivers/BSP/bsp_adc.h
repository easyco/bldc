#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f1xx_hal.h"
#include "bldc.h"

#define ADC_BEMF_A_CH					ADC_CHANNEL_7 
#define ADC_BEMF_B_CH					ADC_CHANNEL_6 
#define ADC_BEMF_C_CH					ADC_CHANNEL_5 
#define ADC_BEMF_REF_CH					ADC_CHANNEL_3
#define ADC_CURRENT_CH					ADC_CHANNEL_8
#define ADC_SPEED_CH					ADC_CHANNEL_9
#define ADC_VBUS_CH						ADC_BEMF_REF_CH	

#define ADCx							ADC1
#define ADCx_CLK_Enable()				__HAL_RCC_ADC1_CLK_ENABLE()

#define ADC_SAMPLE_TIME					ADC_SAMPLETIME_1CYCLE_5

#if (BEMF_REF_MODE == BEMF_REF_MODE_SAMPLE_ALL)
#define ADC_FAST_CHANNEL_NUM			3
#endif

//#define USE_INJECTED_GROUP_SMAPLE_BEMF

#define DMA_EnableTCIE()				(DMA1_Channel1->CCR |= DMA_CCR_TCIE)
#define DMA_DisableTCIE()				(DMA1_Channel1->CCR &= ~DMA_CCR_TCIE)

void ADC_Init(void);

#endif
/* end of file */
