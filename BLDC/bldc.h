#ifndef __BLDC_H
#define __BLDC_H

#include <stdint.h>
#include <stdbool.h>

//#include "bsp_tim.h"

#define MOTOR_START_PWM_DUTY			25
#define MOTOR_START_COMM_PERIOD			20000
#define ZERO_CROSS_CHECK_NUM			2	

/* PWM频率 */
#define BLDC_PWM_FREQ					24000
/* PWM驱动方式 */
#define USE_SYNC_DRIVER

/* BEMF检测方式 */
#define BEMF_REF_MODE_HALF_VBUS			0	//  1/2 VBUS方式
#define BEMF_REF_MODE_MID_POINT			1	//  三电阻虚拟中点方式，用于PWM_ON_OFF方式
#define BEMF_REF_MODE_THRESHOD			2	//	比较值固定，用于PWM_OFF方式
#define BEMF_REF_MODE_ON_PHASE			3	//  直接从PWM_ON的绕组上采样VBUS，这种方式的ADC通道可以减少一路
#define BEMF_REF_MODE_SAMPLE_ALL        4   //  3个绕组都采样，然后计算平均电压就得到绕组中点电压

/* BEMF检测方式 */
#define BEMF_DETECT_AT_PWM_ON			0
#define BEMF_DETECT_AT_PWM_OFF			1
#define BEMF_DETECT_AT_PWM_ON_OFF		2

#define BEMF_REF_MODE					BEMF_REF_MODE_SAMPLE_ALL
#define BEMF_DETECT_TIME				BEMF_DETECT_AT_PWM_ON

typedef enum
{
	MOTOR_STATUS_IDLE = 0,
	MOTOR_STATUS_STOP,
	MOTOR_STATUS_START,
	MOTOR_STATUS_RESTART,
	MOTOR_STATUS_RUNNING,
}   MotorStatus_t;

typedef enum
{
	MOTOR_PHASE_0 = 0,
	MOTOR_PHASE_1 = 1,
	MOTOR_PHASE_2 = 2,
	MOTOR_PHASE_3 = 3,
	MOTOR_PHASE_4 = 4,
	MOTOR_PHASE_5 = 5,
}   MotorPhase_t;

typedef enum
{
	MOTOR_RUNNING_CW  = 0,			// 顺时针
	MOTOR_RUNNING_CCW = 1			// 逆时针
}	MotorRunningDir_t;

typedef struct
{
	uint8_t OverCurrent;
	uint8_t CommEvent;
}   BldcFlag_t;

typedef enum
{
	BEMF_UP = 0,
	BEMF_DOWN = 1
}   BemfDir_t;

typedef struct
{
	MotorStatus_t		MotorStatus;
	MotorRunningDir_t	MotorRunDir;
#if defined (MOTOR_POLE_PAIRS)
	uint16_t			MotorRpm;
#endif
	MotorPhase_t		MotorPhase;
	uint16_t            SpeedSet;
	uint16_t			PwmPulse;				/* PWM脉宽 */
	uint16_t			CommPeriod;				/* 换向周期 */
	uint16_t			AdcBuffer[12];
	uint16_t			ZeroCrossFirstTime;		/* 第一次过零的时间点 */
	uint8_t				ZeroCrossCnt;			/* 过零次数 */
	uint8_t				DemagnCnt;				/* 消磁等待计数器 */
	bool				FlagZeroCross;
}   BldcStatus_t;

extern BldcStatus_t Bldc;

void BLDC_CommCallback(void);
void BLDC_CheckZeroCrossCallback(void);
void BLDC_TaskCallback(void);

#endif
/* end of file */
