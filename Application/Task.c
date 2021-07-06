#include "Sys.h"
#include "Uart.h"
#include "LCD1602.h"
#include "Task.h"
#include "Thermistortables.h"
#include <math.h>
#include "STC12(ADC flag).h"
#include "cfg.h"
#include "Timer.h"

extern float Rt;	//����⵽����������ֵ
extern float Temp;//�������¶�

extern uint8 Dat_Save_flag;

uint16 Set_temp = 0;//����Ŀ���¶�

uint8 Data_pre_Flag = 0;//���ݱ���Flag,ȫ�ֱ���
uint8 SW_flag = 0;			//Ϊ1ʱ������ʼ�¿س���

extern uint8 Data_preservation(uint16 dat);//�����趨�¶ȵ�EEPROM
extern uint16 Get_Temp_set(void);

//extern void EEPROM_Test(void);
void setup(void)
{
	Uart_Init();				//���ڳ�ʼ��
	Init_ADC();  				//ADת����ʼ��
	LCD_Init();         // ��ʼ��LCD
	Timer0_Init();			//��ʱ����ʼ��
	Beep = 0;						//��ʼ��������
	BED = OFF;						//��ʼ���ȴ�
	SW_flag = 1;				//�����������
	LCD_Write_String(2,0,"<Geek Start>");					//��������
	LCD_Write_String(2,1,"GSH_Bed V1.0");
	
	if((Get_Temp_set()>0)&&(Get_Temp_set()<271))	//��ȡ����֤�����¶ȶ�ȡ��ȷ
	{Set_temp = Get_Temp_set();}
	else{Set_temp = 0;}
	printf("Set: %d \r\n",Set_temp);//���������ǰ�¶�
	
	delay_ms(2000);
	LCD_Clear();
	LCD_Write_String(0,0,"Set:");	//��ʾ�趨�¶�	//����������ʾ
	LCD_TEM(7,0,Set_temp);//�����¶�ֵ��ʾ
	LCD_Write_String(0,1,"Temp:");

#ifdef TEST_EEPROM//�������α���
	EEPROM_Test();
#endif
//	if(Data_preservation(270))//���ݱ���
//	{
//		printf("OK \r\n");
//	}
//	printf("Get:%d \r\n",Get_Temp_set());
}
void loop(void)
{
	while(1)//�¶ȿ���
	{
		uint16 Flash_date;		
		if(Sys_SW ==1 )
		{
			Tem_calculation();//�ɼ���ǰ�¶�
			//�򵥵��¶ȿ���
			if(SW_flag == 1)//�����ǰ������ȣ���ʼ�¶ȿ���
			{
				if((Temp<(Set_temp+7))&&(Temp>0))//С���趨�¶�7�ȿ�ʼ����
				{
					BED = ON;
				}else if((Temp>Set_temp)||(Temp==0)||(Temp<0))//�����趨�¶�ֹͣ����
				{
					BED = OFF;
				}
			}
			if(Temp<0)//��СֵΪ0
			{Temp=0;}
			LCD_TEM(7,0,Set_temp);		//��ʾ�趨�¶�ֵ
			LCD_TEM(7,1,Temp);				//��ʾ��ǰ�¶�ֵ
			/**********************��Ҫ���룬�޸Ŀ����������д��EEPROM����оƬ���***************************/
			////�ȶ�ȡ������֤��Ȼ���ٽ���д�룬�������ظ�д��
			if(Dat_Save_flag==1)
			{
				Flash_date = Get_Temp_set();//��ȡ����
				if(Flash_date != Set_temp)//���������һ���ģ�������,����ı���Ŀ���¶��򽫵�ǰ����д��Flash
				{
					Data_preservation(Set_temp);
					printf("save success.%d \r\n",Set_temp);
				}
				Dat_Save_flag=0;
			}
			/************************************************/
//		printf("Set: %d \r\n",Set_temp);//���������ǰ�¶ȡ�
//			printf("Set: %f \r\n",Rt);//���������ǰ��ֵ
//		printf("T: %f \r\n",Temp);//���������ǰ�¶�
			delay_ms(100);//���0.1��ˢ��һ����Ļ			
		}
		else
		{
			BED = OFF;
			Beep = 1;			 //��ʱ�ر��ȴ�ǰ����������һ��
			delay_ms(1000);
			Beep = 0;
			LCD_Clear();
		}
	}
}

// void LCD_V(uint8 a,uint8 l,uint8 p)					//��ʾ��ͨ������õĵ�ѹֵ����Ҫ��P1.0���ż�TL431�����ı�׼��ѹ2.47V���ܹ���������������֤����׼ȷ�������0.02V����
// {
//	LCD_Write_Char(a,l,(int)(Filtering_Voltage(p))/10+0x30);     //��λ
//	LCD_Write_Char(a+1,l,(int)(Filtering_Voltage(p))%10+0x30);     //��λ
//	LCD_Write_Char(a+2,l,'.');
//	LCD_Write_Char(a+3,l,(int)(Filtering_Voltage(p)*100)%100/10+0x30); //ʮ��λ
//	LCD_Write_Char(a+4,l,(int)(Filtering_Voltage(p)*100)%100%10+0x30); //�ٷ�λ
//	LCD_Write_Char(a+5,l,'V');
// }
void LCD_TEM(uint8 a,uint8 b,float p)//�¶�ֵ��ʾ
{
	if(((int)p/100)==0)//�����λ0��ʾ
	{
		LCD_Write_Char(a,b,' ');
		LCD_Write_Char(a+1,b,' ');
	}
	else
	{LCD_Write_Char(a,b,(int)p/100+0x30);}//��λ
	
	if((((int)p%100)/10)!=0)
	{LCD_Write_Char(a+1,b,((int)p%100)/10+0x30);}//ʮλ
	
	LCD_Write_Char(a+2,b,(int)p%10+0x30);     //��λ
	LCD_Write_Char(a+4,b,'C');
}
//
