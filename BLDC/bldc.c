#include <stdbool.h>

#include "bldc.h"
#include "stm32f1_easyco.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "system.h"
#include "comm.h"
#include "pwm.h"
#include "adc.h"

BldcStatus_t Bldc;
static volatile uint32_t zTime;
static volatile uint16_t bRef;
static volatile uint16_t bemf;

const uint8_t BemfDirTable[2][6] = { 
	{ BEMF_DOWN, BEMF_UP, BEMF_DOWN, BEMF_UP, BEMF_DOWN, BEMF_UP },		// CW
	{ BEMF_UP, BEMF_DOWN, BEMF_UP, BEMF_DOWN, BEMF_UP, BEMF_DOWN } };   // CCW

const MotorPhase_t NextPhaseTable[2][6] = {
	{ MOTOR_PHASE_1, MOTOR_PHASE_2, MOTOR_PHASE_3, MOTOR_PHASE_4, MOTOR_PHASE_5, MOTOR_PHASE_0 },	// CW
	{ MOTOR_PHASE_5, MOTOR_PHASE_0, MOTOR_PHASE_1, MOTOR_PHASE_2, MOTOR_PHASE_3, MOTOR_PHASE_4 } }; // CCW

const uint8_t BemfAdcIndexTable[6] = { 2,1,0,2,1,0 };
const uint8_t RefAdcIndex = 3;

extern void BLDC_SetPwmDuty(uint16_t duty);
extern void BLDC_CommPWM(uint8_t phase);
extern bool Button_IsPush(void);

static bool BLDC_IsZeroCross(void)
{
#if (BEMF_REF_MODE == BEMF_REF_MODE_SAMPLE_ALL)
	bRef = (Bldc.AdcBuffer[0] + Bldc.AdcBuffer[1] + Bldc.AdcBuffer[2]) / 3;
#elif (BEMF_REF_MODE == BEMF_REF_MODE_MID_POINT)
	bRef = Bldc.AdcBuffer[RefAdcIndex];
#endif
	if (BemfDirTable[Bldc.MotorRunDir][Bldc.MotorPhase] == BEMF_DOWN)
	{
		if (Bldc.AdcBuffer[BemfAdcIndexTable[Bldc.MotorPhase]] < bRef)
		{
			LED7_On();
			return true;
		}
		else
			return false;
	}
	else
	{
		if (Bldc.AdcBuffer[BemfAdcIndexTable[Bldc.MotorPhase]] > bRef)
		{
			LED7_Off();
			return true;
		}
		else
			return false;
	}
}

/* 换向中断回调函数
*  该函数只有在电机运行状态执行；
*/
void BLDC_CommCallback(void)
{
	//uint16_t speedSet;
	//speedSet = Bldc.AdcBuffer[3]

	Bldc.MotorPhase = NextPhaseTable[Bldc.MotorRunDir][Bldc.MotorPhase];
	BLDC_CommPWM(Bldc.MotorPhase);

	Bldc.FlagZeroCross = false;
	Bldc.ZeroCrossCnt = 0;
	Bldc.DemagnCnt = 2;
	
	BLDC_StopAutoComm();
	BLDC_StartAdc();
}

/* 过零检测回调函数
*  该函数在DMA传输结束中断中调用；
*/
void BLDC_CheckZeroCrossCallback(void)
{
	/* 如果已经检测到过零事件，等待换向 */
	if (Bldc.FlagZeroCross == true)	return;
	/* 等待消磁 */
	if (Bldc.DemagnCnt > 0)
	{
		Bldc.DemagnCnt--;
		return;
	}

	zTime = ZCNT_TIM->CNT;
	if (Bldc.ZeroCrossCnt == 0) Bldc.ZeroCrossFirstTime = zTime;
	
	if (BLDC_IsZeroCross() == true) Bldc.ZeroCrossCnt++;
	else Bldc.ZeroCrossCnt = 0;
	
	if (Bldc.ZeroCrossCnt >= ZERO_CROSS_CHECK_NUM)
	{
		ZCNT_TIM->CNT = 0;
		Bldc.CommPeriod = zTime;
		zTime -= (zTime - Bldc.ZeroCrossFirstTime);
		zTime >>= 1;
		BLDC_UpdateCommPeriod(zTime);
		BLDC_StartAutoComm();
		BLDC_StopAdc();
		Bldc.CommPeriod += zTime;
		Bldc.FlagZeroCross = true;
		LED9_Toggle();
		ADC1->CR2 |= ADC_CR2_JSWSTART;
	}
	
	if (ZCNT_TIM->CNT > 60000)
	{
		if (Bldc.MotorStatus == MOTOR_STATUS_START) 
			Bldc.MotorStatus = MOTOR_STATUS_RESTART;
	}
}

bool BLDC_StartUp(void)
{
	static uint16_t n = 0;
	
	printf("Motor startup ...\r\n");
	n++;

	Bldc.CommPeriod = MOTOR_START_COMM_PERIOD;
	Bldc.ZeroCrossCnt = 0;
	Bldc.FlagZeroCross = false;
	Bldc.DemagnCnt = 0;

	Bldc.MotorPhase = NextPhaseTable[Bldc.MotorRunDir][Bldc.MotorPhase];
	
	BLDC_UpdatePwmDuty(Bldc.PwmPulse);
	BLDC_CommPWM(Bldc.MotorPhase);
	BLDC_StartPWM();

	BLDC_UpdateCommPeriod(Bldc.CommPeriod);
	BLDC_StartAutoComm();

	ZCNT_TIM->CNT = 0;
	Bldc.FlagZeroCross = true;

	TIM1->SR &= ~TIM_FLAG_UPDATE;
	TIM1->SR &= ~TIM_FLAG_CC4;
	PWM_TIM->DIER |= TIM_FLAG_UPDATE;
	PWM_TIM->DIER |= TIM_DIER_CC4IE;

	BLDC_StartAdc();

	printf("Motor running ...\r\n");
	return true;
}

#if defined (MOTOR_POLE_PAIRS)
uint32_t BLDC_GetMotorSpeed(void)
{
	uint32_t freq, rpm = 0;

	freq = (uint32_t)(SystemCoreClock / TIM2->PSC) / (Bldc.CommPeriod * 6);
	rpm = (uint32_t)((freq * 60) / MOTOR_POLE_PAIRS);

	return rpm;
}
#endif

void BLDC_TaskCallback(void)
{
	static uint16_t taskCnt = 0;
	static uint8_t buttonCnt = 0;
	
	taskCnt++;

	if (Bldc.MotorStatus == MOTOR_STATUS_STOP)
	{
		BLDC_StopPWM();
		/* 禁止PWM更新中断 */
		PWM_TIM->DIER &= ~0x01;
		/* 禁止PWM CC4比较中断 */
		PWM_TIM->DIER &= ~TIM_DIER_CC4IE;
		/* 禁止换向中断 */
		BLDC_StopAutoComm();
		/* 禁止DMA中断 */
		BLDC_StopAdc();

		LED7_Off();
		LED8_Off();
		LED9_Off();
		Bldc.MotorStatus = MOTOR_STATUS_IDLE;
	}
	else if (Bldc.MotorStatus == MOTOR_STATUS_START)
	{
		if (taskCnt > 500) Bldc.MotorStatus = MOTOR_STATUS_RUNNING;
	}
	else if (Bldc.MotorStatus == MOTOR_STATUS_RESTART)
	{
		//printf("Motor restart ...\r\n");
		if (Bldc.PwmPulse < PWM_TIM_PERIOD / 3) Bldc.PwmPulse += PWM_TIM_PERIOD/20;
		Bldc.MotorStatus = MOTOR_STATUS_START;
		BLDC_StartUp();
	}
	else if (Bldc.MotorStatus == MOTOR_STATUS_RUNNING)
	{
		//if (Bldc.PwmPulse > (uint32_t)(MOTOR_START_PWM_DUTY * PWM_TIM_PERIOD) / 100) Bldc.PwmPulse -= PWM_TIM_PERIOD / 20;
		//BLDC_UpdatePwmDuty(Bldc.PwmPulse);

		Bldc.SpeedSet = Bldc.AdcBuffer[3];
		Bldc.SpeedSet = ((uint32_t)Bldc.SpeedSet * 100) / 4095;
		if (Bldc.SpeedSet > MOTOR_START_PWM_DUTY)
		{
			Bldc.PwmPulse = ((uint32_t)Bldc.SpeedSet * PWM_TIM_PERIOD) / 100;
			BLDC_UpdatePwmDuty(Bldc.PwmPulse);
		}

		if (taskCnt % 1000 == 0)
		{
			printf("SpeedSet = %d\r\n",Bldc.AdcBuffer[3]);
		}
	}

	if (taskCnt % 10 == 0)
	{
		if (Button_IsPush() == true)
		{
			buttonCnt++;
		}
		else
		{
			if (buttonCnt > 5)
			{
				buttonCnt = 0;
				if (Bldc.MotorStatus == MOTOR_STATUS_IDLE)
				{
					Bldc.MotorStatus = MOTOR_STATUS_START;
					Bldc.MotorRunDir = MOTOR_RUNNING_CCW;
					Bldc.MotorPhase = MOTOR_PHASE_0;
					Bldc.PwmPulse = (uint32_t)(MOTOR_START_PWM_DUTY * PWM_TIM_PERIOD) / 100;
					BLDC_StartUp();
					taskCnt = 0;
				}
				else if ((Bldc.MotorStatus == MOTOR_STATUS_RUNNING) || (Bldc.MotorStatus == MOTOR_STATUS_START))
				{
					Bldc.MotorStatus = MOTOR_STATUS_STOP;
				}
			}
		}
	}
}
/* end of file */
