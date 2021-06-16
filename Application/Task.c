#include "Sys.h"
#include "Uart.h"
#include "LCD1602.h"
#include "Task.h"
#include "STC12(ADC flag).h"
#include "cfg.h"
#include	"math.h"
void setup(void)
{
	LCD_Init();         // ��ʼ��LCD
	Uart_Init();				//���ڳ�ʼ��
	Init_ADC();  				//ADת����ʼ��
	Beep = 0;
	
//	LCD_Write_String(0,0,"GSH-BED");
//	printf("GSH");
//		LCD_Clear(); 
		LCD_Write_String(0,0,"sys_V:");	//��ʾϵͳ������ѹ
		LCD_V(6,0,1);
printf("%f",(int)Filtering_Voltage(1)*6);
}
void loop(void)
{
	while(1)
	{
		
	}
}
 void LCD_V(uint8 a,uint8 l,uint8 p)					//��ʾ��ͨ������õĵ�ѹֵ����Ҫ��P1.0���ż�TL431�����ı�׼��ѹ2.47V���ܹ���������������֤����׼ȷ�������0.02V����
 {
	LCD_Write_Char(a,l,(int)(Filtering_Voltage(p)*6)/10+0x30);     //��λ
	LCD_Write_Char(a+1,l,(int)(Filtering_Voltage(p)*6)%10+0x30);     //��λ
	LCD_Write_Char(a+2,l,'.');
	LCD_Write_Char(a+3,l,(int)(Filtering_Voltage(p)*6*100)%100/10+0x30); //ʮ��λ
	LCD_Write_Char(a+4,l,(int)(Filtering_Voltage(p)*6*100)%100%10+0x30); //�ٷ�λ
	LCD_Write_Char(a+5,l,'V');
 }
