#include "bsp_adc.h"
#include "bldc.h"

ADC_HandleTypeDef hadc1;

static void ADC_InitDMA(void)
{
	static DMA_HandleTypeDef  DmaHandle;

	__HAL_RCC_DMA1_CLK_ENABLE();

	DmaHandle.Instance = DMA1_Channel1;

	DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DmaHandle.Init.Mode = DMA_CIRCULAR;
	DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&DmaHandle);
	__HAL_LINKDMA(&hadc1, DMA_Handle, DmaHandle);

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void ADC_Init(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit;
	ADC_ChannelConfTypeDef   sConfig;
	ADC_InjectionConfTypeDef sConfigInjected;

#if !defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
	ADC_InitDMA();
#endif

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	ADCx_CLK_Enable();

#if defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
	hadc1.Instance = ADCx;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = ADC_FAST_CHANNEL_NUM;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	HAL_ADC_Init(&hadc1);

	ADC1->CR2 |= ADC_CR2_JEXTTRIG;							

	sConfigInjected.InjectedOffset = 0;
	sConfigInjected.InjectedNbrOfConversion = ADC_FAST_CHANNEL_NUM;
	sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
	sConfigInjected.AutoInjectedConv = DISABLE;
	sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T4_TRGO;
	
	sConfigInjected.InjectedChannel = ADC_BEMF_A_CH;
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLE_TIME;
	HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);

	sConfigInjected.InjectedChannel = ADC_BEMF_B_CH;
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_2;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLE_TIME;
	HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);

	sConfigInjected.InjectedChannel = ADC_BEMF_C_CH;
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_3;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLE_TIME;
	HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);

	HAL_NVIC_SetPriority(ADC1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);
#else
	hadc1.Instance = ADCx;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = ADC_FAST_CHANNEL_NUM;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	HAL_ADC_Init(&hadc1);

	ADC1->CR2 |= ADC_CR2_EXTTRIG;							

	sConfig.Channel = ADC_BEMF_A_CH;
	sConfig.SamplingTime = ADC_SAMPLE_TIME;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_BEMF_B_CH;
	sConfig.SamplingTime = ADC_SAMPLE_TIME;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_BEMF_C_CH;
	sConfig.SamplingTime = ADC_SAMPLE_TIME;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR = (uint32_t)&Bldc.AdcBuffer;
	/* DMA data length */
	DMA1_Channel1->CNDTR = ADC_FAST_CHANNEL_NUM;

	/* DMA Enable */
	DMA1_Channel1->CCR |= DMA_CCR_EN;

	ADC1->CR2 |= ADC_CR2_DMA;

	sConfigInjected.InjectedOffset = 0;
	sConfigInjected.InjectedNbrOfConversion = 1;
	sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
	sConfigInjected.AutoInjectedConv = DISABLE;
	sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;

	sConfigInjected.InjectedChannel = ADC_SPEED_CH;
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_13CYCLES_5;
	HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);

	HAL_NVIC_SetPriority(ADC1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);

	ADC1->CR2 |= ADC_CR2_JEXTTRIG;							
	ADC1->CR1 |= ADC_CR1_JEOSIE;
	ADC1->CR2 |= ADC_CR2_JSWSTART;
#endif
	
	HAL_ADCEx_Calibration_Start(&hadc1);

#if defined (USE_INJECTED_GROUP_SMAPLE_BEMF)
	ADC1->CR1 |= ADC_CR1_JEOSIE;
	ADC1->CR2 |= ADC_CR2_JSWSTART;
#else
	ADC1->CR2 |= ADC_CR2_SWSTART;
#endif
}
/* end of file */
