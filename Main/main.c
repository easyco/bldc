/**
******************************************************************************
* @project/file		STM32 BLDC
* @author			xiaopeng
* @version			V0.0.0
* @date				21-December-2017
* @brief			STM32 + EASYCO BLDC DEVELOP KIT V2.1 Brushless motor driver
*
*
******************************************************************************
* @attention / history
*
*
*
******************************************************************************
*/
#include "system.h"
#include "stm32f1_easyco.h"

int main(void)
{
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_NOJTAG();
	__HAL_AFIO_REMAP_USART1_ENABLE();

	HAL_Init();
	SystemClock_Config();

	BSP_Init();

	UART_Init();
	ADC_Init();
	TIM_InitPwm(); 
	TIM_InitComm();
	TIM_InitCrossCnt();
	TIM_InitAdcTrigger();

	while (1)
	{
	}
}
/* end of file */
