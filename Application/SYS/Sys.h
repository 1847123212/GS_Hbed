#ifndef __SYS_H
#define __SYS_H
#include "STC12C5A60S2.h"
#include <intrins.h>
#define uint8 	unsigned char
#define uint16 	unsigned int
#define nops();  {_nop_(); _nop_(); _nop_(); _nop_();} //�����ָ��
void delay_11us(uint16 n);					//��������
void delay_ms(uint16 n);						//��ʱ����
#endif