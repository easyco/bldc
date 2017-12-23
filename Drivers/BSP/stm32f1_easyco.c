#include "system.h"
#include "stm32f1_easyco.h"

UART_HandleTypeDef huart;

static void BSP_InitLED(void)
{
	GPIO_InitTypeDef gpioInit;

	LED_GPIO_CLK_Enable();
	
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInit.Pin = (LED7_GPIO_PIN | LED8_GPIO_PIN | LED9_GPIO_PIN);
	HAL_GPIO_Init(LED_GPIO_PORT, &gpioInit);

	LED7_Off();
	LED8_Off();
	LED9_Off();
}

static void BSP_InitButton(void)
{
	GPIO_InitTypeDef gpioInit;

	BUTTON_GPIO_CLK_Enable();

	gpioInit.Mode = GPIO_MODE_INPUT;
	gpioInit.Pull = GPIO_PULLUP;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
	gpioInit.Pin = BUTTON_GPIO_PIN;
	HAL_GPIO_Init(BUTTON_GPIO_PORT, &gpioInit);
}

static void BSP_InitUART(void)
{
	GPIO_InitTypeDef gpioInit;

	UART_GPIO_CLK_Enable();
	
	gpioInit.Mode = GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_PULLUP;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInit.Pin = UART_TX_GPIO_PIN | UART_RX_GPIO_PIN;
	HAL_GPIO_Init(UART_GPIO_PORT, &gpioInit);
}

static void BSP_InitPWM(void)
{
	GPIO_InitTypeDef gpioInit;

	PWM_H_GPIO_CLK_Enable();
	PWM_L_GPIO_CLK_Enable();

	gpioInit.Pin = PWM_AH_GPIO_PIN | PWM_BH_GPIO_PIN | PWN_CH_GPIO_PIN;
	gpioInit.Mode = GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PWM_H_GPIO_PORT, &gpioInit);

	gpioInit.Pin = PWM_AL_GPIO_PIN | PWM_BL_GPIO_PIN | PWN_CL_GPIO_PIN;
	gpioInit.Mode = GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PWM_L_GPIO_PORT, &gpioInit);
}

static void BSP_InitADC(void)
{
	GPIO_InitTypeDef gpioInit;

	ADC_BEMF_GPIO_CLK_Enable();
	ADC_VBUS_GPIO_CLK_Enable();
	ADC_CURRENT_GPIO_CLK_Enable();
	ADC_SPEED_GPIO_CLK_Enable();

	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;

	gpioInit.Pin = ADC_BEMF_A_GPIO_PIN | ADC_BEMF_B_GPIO_PIN | ADC_BEMF_C_GPIO_PIN;
	HAL_GPIO_Init(ADC_BEMF_GPIO_PORT, &gpioInit);

	gpioInit.Pin = ADC_SPEED_GPIO_PIN;
	HAL_GPIO_Init(ADC_SPEED_GPIO_PORT, &gpioInit);

	gpioInit.Pin = ADC_VBUS_GPIO_PIN;
	HAL_GPIO_Init(ADC_VBUS_GPIO_PORT, &gpioInit);

	gpioInit.Pin = ADC_CURRENT_GPIO_PIN;
	HAL_GPIO_Init(ADC_CURRENT_GPIO_PORT, &gpioInit);
}

void BSP_Init(void)
{
	BSP_InitLED();
	BSP_InitButton();
	BSP_InitUART();
	BSP_InitPWM();
	BSP_InitADC();
}

void UART_Init(void)
{
	UART_CLK_Enable();

	huart.Instance = UART;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&huart);
}

bool Button_IsPush(void)
{
	if (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == GPIO_PIN_RESET)
	{
		return	true;
	}
	else
	{
		return false;
	}
}

/* end of file */
