#include "reg52.h"
#include "Sys.h"
#include "stdio.h"//ʹ��printf��Ҫ
#include "DS18B20.h"
#include "Uart.h"
#include "LCD1602.h"
#include "key.h"
sbit j = P1^7;
sbit i = P1^6;
sbit Beep = P0^4;
sbit BED	= P1^3;
void main()
{
	LCD_Init();         // ��ʼ��LCD
	LCD_Clear();				//����
	Uart_Init();				//���ڳ�ʼ��
	Beep = 0;
//	BED = 0;
	LCD_Write_String(0,0,"GSH-BED");
	printf("GSH");
//	EEPROM_Test();
		while(1)
		{
			BED = 1;
//			Beep = 0;
			delay_ms(3000);
			BED = 0;
//			Beep = 1;
			delay_ms(3000);
		}
}