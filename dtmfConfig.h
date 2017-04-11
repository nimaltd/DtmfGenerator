#ifndef	_DTMFCONFIG_H
#define	_DTMFCONFIG_H


#define		_DTMF_USE_FREERTOS				1
#define		_DTMF_TIMER								htim1
#define		_DTMF_OUTPUT_PWM					_DTMF_TIMER.Instance->CCR1
#define		_DTMF_OUTPUT_PWM_CHANNEL	TIM_CHANNEL_1
#define		_DTMF_IRQ									TIM1_UP_IRQn


//#########################################################################################
#if			(_DTMF_USE_FREERTOS==0)
	#define	_DTMF_DELAY(x)							HAL_Delay(x)
	#define	_DTMF_LOCK()								{while(DtmfLock==true);DtmfLock=true;}
	#define	_DTMF_RELASE()							DtmfLock=false;	
#else
	#include "cmsis_os.h"
	#define	_DTMF_DELAY(x)							osDelay(x)
	#define	_DTMF_LOCK()								osSemaphoreWait(DtmfSemHandle,osWaitForever)
	#define	_DTMF_RELASE()							osSemaphoreRelease(DtmfSemHandle)
#endif


#endif

