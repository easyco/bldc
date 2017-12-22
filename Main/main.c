/**
******************************************************************************
* @project/file		stm32f1xx_hal_gpio.c
* @author			MCD Application Team
* @version			V1.0.4
* @date				29-April-2016
* @brief			GPIO HAL module driver.
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

	while (1)
	{
		HAL_Delay(100);
		LED7_Toggle();
		LED8_Toggle();
		LED9_Toggle();
	}
}
/* end of file */
