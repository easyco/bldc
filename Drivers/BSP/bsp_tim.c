#include "bsp_tim.h"
#include "bldc.h"

TIM_HandleTypeDef htimPwm;
TIM_HandleTypeDef htimComm;
TIM_HandleTypeDef htimAdc;

void TIM_InitPwm(void)
{
	TIM_OC_InitTypeDef sConfigOC;
	TIM_MasterConfigTypeDef sMasterConfig;

	PWM_CLK_Enable();

	htimPwm.Instance = PWM_TIM;
	htimPwm.Init.ClockDivision = 0;
	htimPwm.Init.Prescaler = PWM_TIM_PRESCALER;
	htimPwm.Init.Period = PWM_TIM_PERIOD;
	htimPwm.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimPwm.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&htimPwm);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = PWM_TIM_PERIOD / 10;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&htimPwm, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htimPwm, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htimPwm, &sConfigOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htimPwm, &sConfigOC, TIM_CHANNEL_4);

	/* PWM CH and CHN dead time */
	PWM_TIM->BDTR |= PWM_DEADTIME;

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC4REF;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htimPwm, &sMasterConfig);
}

void TIM_InitComm(void)
{
	COMM_TIM_CLK_Enable();

	htimComm.Instance = COMM_TIM;
	htimComm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimComm.Init.Prescaler = COMM_TIM_PRESCALER;
	htimComm.Init.Period = COMM_TIM_PERIOD;
	htimComm.Init.RepetitionCounter = 0;
	htimComm.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htimComm);

	COMM_TIM->DIER |= TIM_IT_UPDATE;
	HAL_NVIC_SetPriority(COMM_TIM_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(COMM_TIM_IRQn);
}

void TIM_InitCrossCnt(void)
{
	static TIM_HandleTypeDef htimZcnt;

	ZCNT_TIM_CLK_Enable();

	htimZcnt.Instance = ZCNT_TIM;
	htimZcnt.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimZcnt.Init.Prescaler = ZCNT_TIM_PRESCALER;
	htimZcnt.Init.Period = ZCNT_TIM_PERIOD;
	htimZcnt.Init.RepetitionCounter = 0;
	htimZcnt.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htimZcnt);

	HAL_TIM_Base_Start(&htimZcnt);
}

void TIM_InitAdcTrigger(void)
{
	TIM_OC_InitTypeDef sConfigOC;
	TIM_SlaveConfigTypeDef sSlaveConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	ADC_TRIG_TIM_CLK_Enable();

	htimAdc.Instance = ADC_TRIG_TIM;
	htimAdc.Init.Prescaler = ADC_TRIG_TIM_PRESCALER;
	htimAdc.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimAdc.Init.Period = ADC_TRIG_TIM_PERIOD;
	htimAdc.Init.ClockDivision = 0;
	htimAdc.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&htimAdc);

	/* 门控模式，触发信号来自TIM1 */
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_GATED;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	HAL_TIM_SlaveConfigSynchronization(&htimAdc, &sSlaveConfig);

	/* UPDATE事件触发ADC转换 */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htimAdc, &sMasterConfig);
}
/* end of file */
