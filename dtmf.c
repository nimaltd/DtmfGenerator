
#include "dtmf.h"
#include "dtmfConfig.h"
#include "tim.h"
#include <stdbool.h>
#include <string.h>

#if			(_DTMF_USE_FREERTOS==0)
bool	DtmfLock=false;
#else
osSemaphoreId DtmfSemHandle;
#endif

uint8_t dtmf_low = 0;
uint8_t dtmf_high = 0;

const uint8_t dtmf_code_high[] = {0, 79, 87, 96, 107};
const uint8_t dtmf_code_low[] = {0, 46, 50, 56, 61};

const uint8_t dtmf_sine[128] =
{
	64,67,70,73,76,79,82,85,88,91,94,96,99,102,104,106,109,111,113,115,117,118,
	120,121,123,124,125,126,126,127,127,127,127,127,127,127,126,126,125,124,123,121,
	120,118,117,115,113,111,109,106,104,102,99,96,94,91,88,85,82,79,76,73,70,67,
	64,60,57,54,51,48,45,42,39,36,33,31,28,25,23,21,18,16,14,12,10,9,7,6,4,3,2,1,1,0,0,0,0,0,
	0,0,1,1,2,3,4,6,7,9,10,12,14,16,18,21,23,25,28,31,33,36,39,42,45,48,51,54,57,60
};

#define DTMF_697     0x01
#define DTMF_770     0x02
#define DTMF_852     0x03
#define DTMF_941     0x04

#define DTMF_1209    0x10
#define DTMF_1336    0x20
#define DTMF_1477    0x30
#define DTMF_1633    0x40

//#####################################################################################################
void	Dtmf_Init(void)
{
	HAL_TIM_PWM_Start(&_DTMF_TIMER,_DTMF_OUTPUT_PWM_CHANNEL);	
	HAL_TIM_Base_Start_IT(&_DTMF_TIMER);
	#if	(_DTMF_USE_FREERTOS==1)
	osSemaphoreDef(DtmfSemHandle);
	DtmfSemHandle = osSemaphoreCreate(osSemaphore(DtmfSemHandle), 1);
	#endif
	HAL_NVIC_DisableIRQ(_DTMF_IRQ);
	_DTMF_OUTPUT_PWM=0;
	
}
//#####################################################################################################
void	Dtmf_CallBack(void)
{
	static uint16_t low_val = 0;
	static uint16_t high_val = 0;

	uint8_t low_ptr;
	uint8_t high_ptr;

	low_val += dtmf_low;
	high_val += dtmf_high;

	low_ptr = ((low_val + 4) >> 3) & (0x007F);
	high_ptr = ((high_val + 4) >> 3) & (0x007F);

	// high + 3/4 low
	_DTMF_OUTPUT_PWM = dtmf_sine[high_ptr] + (dtmf_sine[low_ptr] - (dtmf_sine[low_ptr] >> 2));
}
//#####################################################################################################
bool 	Dtmf_S(char CharCode,uint16_t SendingTime)
{	
	uint8_t code;
	switch (CharCode)
	{
		case '0':
			code = DTMF_941 + DTMF_1336;
		break;
		case '1':
			code = DTMF_697 + DTMF_1209;
		break;
		case '2':
			code = DTMF_697 + DTMF_1336;
		break;
		case '3':
			code = DTMF_697 + DTMF_1477;
		break;
		case '4':
			code = DTMF_770 + DTMF_1209;
		break;
		case '5':
			code = DTMF_770 + DTMF_1336;
		break;
		case '6':
			code = DTMF_770 + DTMF_1477;
		break;
		case '7':
			code = DTMF_852 + DTMF_1209;
		break;
		case '8':
			code = DTMF_852 + DTMF_1336;
		break;
		case '9':
			code = DTMF_852 + DTMF_1477;
		break;
		case '*':
			code = DTMF_941 + DTMF_1209;
		break;
		case '#':
			code = DTMF_941 + DTMF_1477;
		break;
		case 'A':
		case 'a':
			code = DTMF_697 + DTMF_1633;
		break;
		case 'B':
		case 'b':
			code = DTMF_770 + DTMF_1633;
		break;	
		case 'C':
		case 'c':			
			code = DTMF_852 + DTMF_1633;
		break;	
		case 'D':
		case 'd':
			code = DTMF_941 + DTMF_1633;
		break;		
		default:
			HAL_NVIC_DisableIRQ(_DTMF_IRQ);
			_DTMF_OUTPUT_PWM=0;
			return false;
	}	
	dtmf_low = dtmf_code_low[code & 0x0F];
	dtmf_high = dtmf_code_high[(code >> 4) & 0x0F];
	HAL_NVIC_EnableIRQ(_DTMF_IRQ);
	while(SendingTime>0)
	{
		_DTMF_DELAY(1);
		SendingTime--;
	}
	HAL_NVIC_DisableIRQ(_DTMF_IRQ);
	_DTMF_OUTPUT_PWM=0;
	return true;
}
//#####################################################################################################
void 	Dtmf_Send(char *CharCodeString,uint16_t	SpaceTime,uint16_t SendingTimeForPerDigit)
{
	_DTMF_LOCK();
	for(uint8_t i=0 ; i<strlen(CharCodeString) ;i++)
	{
		if(Dtmf_S(CharCodeString[i],SendingTimeForPerDigit)==true)
			_DTMF_DELAY(SpaceTime);
	}
	_DTMF_RELASE();
}
//#####################################################################################################
void 	Dtmf_SendChar(char CharCode,uint16_t SendingTime)
{
	if(Dtmf_S(CharCode,SendingTime)==true)
		_DTMF_DELAY(SendingTime);	
}
//#####################################################################################################

