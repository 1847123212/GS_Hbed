#ifndef __Timer_H
#define __Timer_H

//��ʱ�����ã���Ҫ���ڿ��Ʋ������
#include "STC12C5A60S2.h"
#include "Sys.h"
void Timer0_Init(void);
extern uint8 Data_pre_Flag;
extern uint16 Set_temp1;
extern uint16 Set_temp2;
extern uint8		Sys_SW;
extern uint16	 MODE;
#endif