#include "Sys.h"
#include "Uart.h"
#include "LCD1602.h"
#include "Task.h"
#include "Thermistortables.h"
#include <math.h>
#include "STC12(ADC flag).h"
#include "cfg.h"
extern float Rt;
extern float Temp;
void setup(void)
{
	LCD_Init();         // ��ʼ��LCD
	Uart_Init();				//���ڳ�ʼ��
	Init_ADC();  				//ADת����ʼ��
	Beep = 0;
	
//	LCD_Write_String(0,0,"GSH-BED");
//	printf("GSH");
//		LCD_Clear(); 
		LCD_Write_String(0,0,"Tem_V:");	//��ʾϵͳ������ѹ
		LCD_Write_String(0,1,"Tem:");
}
void loop(void)
{
	while(1)
	{
		
		printf("V: %f \r\n",(float)Filtering_Voltage(2));//
		Tem_calculation();//�¶ȼ���
		printf("R: %f \r\n",Rt);
		printf("T: %f \r\n",Temp);
		LCD_V(7,0,2);//��ѹֵ
		LCD_TEM(7,1,Temp);//�¶�ֵ
		delay_ms(3000);
	}
}

 void LCD_V(uint8 a,uint8 l,uint8 p)					//��ʾ��ͨ������õĵ�ѹֵ����Ҫ��P1.0���ż�TL431�����ı�׼��ѹ2.47V���ܹ���������������֤����׼ȷ�������0.02V����
 {
	LCD_Write_Char(a,l,(int)(Filtering_Voltage(p))/10+0x30);     //��λ
	LCD_Write_Char(a+1,l,(int)(Filtering_Voltage(p))%10+0x30);     //��λ
	LCD_Write_Char(a+2,l,'.');
	LCD_Write_Char(a+3,l,(int)(Filtering_Voltage(p)*100)%100/10+0x30); //ʮ��λ
	LCD_Write_Char(a+4,l,(int)(Filtering_Voltage(p)*100)%100%10+0x30); //�ٷ�λ
	LCD_Write_Char(a+5,l,'V');
 }
void LCD_TEM(uint8 a,uint8 b,float p)
{
	LCD_Write_Char(a,b,(int)p/100+0x30);     //��λ
	LCD_Write_Char(a+1,b,((int)p%100)/10+0x30);     //��λ
	LCD_Write_Char(a+2,b,(int)p%10+0x30);     //��λ
	LCD_Write_Char(a+3,b,'.');
	LCD_Write_Char(a+4,b,(int)(p*100)%100/10+0x30); //ʮ��λ
	LCD_Write_Char(a+5,b,(int)(p*100)%100%10+0x30); //�ٷ�λ
	LCD_Write_Char(a+6,b,'C');
}