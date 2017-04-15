#ifndef	_DTMF_H
#define	_DTMF_H

#include <stdint.h>


void	Dtmf_Init(void);
void	Dtmf_CallBack(void);
void 	Dtmf_Send(char *CharCodeString,uint16_t	SpaceTime,uint16_t SendingTimeForPerDigit);
void 	Dtmf_SendChar(char CharCode,uint16_t SendingTime);

#endif
