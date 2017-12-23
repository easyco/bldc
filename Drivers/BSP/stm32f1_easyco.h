#ifndef __STM32F1_EASYCO_H
#define __STM32F1_EASYCO_H

#include "stm32f1xx_hal.h"
#include "system.h"

#define LED_GPIO_PORT				GPIOB
#define LED7_GPIO_PIN				GPIO_PIN_3
#define LED8_GPIO_PIN				GPIO_PIN_4
#define LED9_GPIO_PIN				GPIO_PIN_5
#define LED_GPIO_CLK_Enable()		__HAL_RCC_GPIOB_CLK_ENABLE()

#define LED7_On()					(GPIOB->BSRR = GPIO_PIN_3)
#define LED7_Off()					(GPIOB->BSRR = (uint32_t)GPIO_PIN_3 << 16)
#define LED7_Toggle()				HAL_GPIO_TogglePin(LED_GPIO_PORT,LED7_GPIO_PIN)
#define LED8_On()					(GPIOB->BSRR = GPIO_PIN_4)
#define LED8_Off()					(GPIOB->BSRR = (uint32_t)GPIO_PIN_4 << 16)
#define LED8_Toggle()				HAL_GPIO_TogglePin(LED_GPIO_PORT,LED8_GPIO_PIN)
#define LED9_On()					(GPIOB->BSRR = GPIO_PIN_5)
#define LED9_Off()					(GPIOB->BSRR = (uint32_t)GPIO_PIN_5 << 16)
#define LED9_Toggle()				HAL_GPIO_TogglePin(LED_GPIO_PORT,LED9_GPIO_PIN)

#define BUTTON_GPIO_PORT			GPIOB
#define BUTTON_GPIO_PIN				GPIO_PIN_8
#define BUTTON_GPIO_CLK_Enable()	__HAL_RCC_GPIOB_CLK_ENABLE()

#define UART_GPIO_PORT				GPIOB
#define UART_TX_GPIO_PIN			GPIO_PIN_6
#define UART_RX_GPIO_PIN			GPIO_PIN_7
#define UART_GPIO_CLK_Enable()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define UART						USART1
#define UART_CLK_Enable()			__HAL_RCC_USART1_CLK_ENABLE()

#define PWM_H_GPIO_PORT				GPIOA
#define PWM_AH_GPIO_PIN				GPIO_PIN_8
#define PWM_BH_GPIO_PIN				GPIO_PIN_9
#define PWN_CH_GPIO_PIN				GPIO_PIN_10
#define PWM_H_GPIO_CLK_Enable()		__HAL_RCC_GPIOA_CLK_ENABLE()

#define PWM_L_GPIO_PORT				GPIOB
#define PWM_AL_GPIO_PIN				GPIO_PIN_13
#define PWM_BL_GPIO_PIN				GPIO_PIN_14
#define PWN_CL_GPIO_PIN				GPIO_PIN_15
#define PWM_L_GPIO_CLK_Enable()		__HAL_RCC_GPIOB_CLK_ENABLE()

#define ADC_BEMF_GPIO_PORT				GPIOA
#define ADC_BEMF_GPIO_CLK_Enable()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_BEMF_A_GPIO_PIN				GPIO_PIN_7
#define ADC_BEMF_B_GPIO_PIN				GPIO_PIN_6
#define ADC_BEMF_C_GPIO_PIN				GPIO_PIN_5

#define ADC_VBUS_GPIO_PORT				GPIOA
#define ADC_VBUS_GPIO_CLK_Enable()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_VBUS_GPIO_PIN				GPIO_PIN_3

#define ADC_CURRENT_GPIO_PORT			GPIOB
#define ADC_CURRENT_GPIO_CLK_Enable()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define ADC_CURRENT_GPIO_PIN			GPIO_PIN_0

#define ADC_SPEED_GPIO_PORT				GPIOB
#define ADC_SPEED_GPIO_CLK_Enable()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define ADC_SPEED_GPIO_PIN				GPIO_PIN_1

//#define ADC_BEMF_A_CH					ADC_CHANNEL_7    
//#define ADC_BEMF_B_CH					ADC_CHANNEL_6   
//#define ADC_BEMF_C_CH					ADC_CHANNEL_5   
//#define ADC_VBUS_CH						ADC_CHANNEL_3
//#define ADC_CURRENT_CH					ADC_CHANNEL_8
//#define ADC_SPEED_CH					ADC_CHANNEL_9

//#define VBUS_ADC_GAIN					768		// 放大100倍			 

#define ADC_INJECTED_TOGO_TIM				TIM4
#define ADC_INJECTED_TOGO_TIM_CLK_Enable()	__HAL_RCC_TIM4_CLK_ENABLE()
#define ADC_INJECTED_TRGO					ADC_EXTERNALTRIGINJECCONV_T4_TRGO

void BSP_Init(void);
void UART_Init(void);
bool Button_IsPush(void);


#endif
/* end of file */
