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

uint16	 MODE = ModeA;
uint16 Set_temp1 = 0;//����Ŀ���¶�
uint16 Set_temp2 = 0;//����Ŀ���¶�

uint8 Data_pre_Flag = 0;//���ݱ���Flag,ȫ�ֱ���
uint8 SW_flag = 0;			//Ϊ1ʱ������ʼ�¿س���

extern uint8 Data_preservation(uint16 dat1,uint16 dat2,uint16 mode);//�����趨�¶ȵ�EEPROM
extern uint16 Get_Temp_set1(void);
extern uint16 Get_Temp_set2(void);
extern uint16 Get_Mode(void);
static uint16 Flash_date1,Flash_date2,Flash_date3;
//extern void EEPROM_Test(void);
void LCD_V(uint8 a,uint8 l,uint8 p);
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
	
	if((Get_Temp_set1()>0)&&(Get_Temp_set1()<271))	//��ȡ����֤�����¶ȶ�ȡ��ȷ
	{Set_temp1 = Get_Temp_set1();}
	else{Set_temp1 = 0;}
	
	if((Get_Temp_set2()>0)&&(Get_Temp_set2()<271))	//��ȡ����֤�����¶ȶ�ȡ��ȷ
	{Set_temp2 = Get_Temp_set2();}
	else{Set_temp2 = 0;}
	MODE = Get_Mode();//��ȡĬ��ģʽ
	printf("Mode: %d \r\n",MODE);//���������ǰ�¶�	
	if((MODE != ModeA)&&(MODE != ModeB))
	{MODE = ModeA;}
	printf("Set1: %d \r\n",Set_temp1);//���������ǰ�¶�
	printf("Set2: %d \r\n",Set_temp2);//���������ǰ�¶�
	
	delay_ms(2000);
	LCD_Clear();
	LCD_Write_String(0,0,"Sys:  .  V Mode ");	//��ʾϵͳ������ѹ�͵�ǰ����ģʽ
	LCD_Write_String(0,1,"Tem:    C->    C");	//��ʾ�趨�¶�	//����������ʾ
	if(MODE == ModeA)
	{
		LCD_TEM(11,1,Set_temp1);
		LCD_Write_String(15,0,"A");//LCD��ʾ��ǰģʽ
	}//�����¶�ֵ��ʾ
	else if(MODE == ModeB)
	{
		LCD_TEM(11,1,Set_temp2);
		LCD_Write_String(15,0,"B");//LCD��ʾ��ǰģʽ
	}
	LCD_V(4,0,1);//��ʾϵͳ�����ѹ
//	EEPROM_Test();
}
void loop(void)
{
	while(1)//�¶ȿ���
	{
		if(Sys_SW ==1 )//ϵͳ�������п���
		{
			Tem_calculation();//�ɼ���ǰ�¶�
			//�򵥵��¶ȿ���
			if(SW_flag == 1)//�����ǰ������ȣ���ʼ�¶ȿ���
			{
				if(MODE == ModeA)//ģʽA�¿�
				{
					LCD_Write_String(15,0,"A");//LCD��ʾ��ǰģʽ
					LCD_TEM(11,1,Set_temp1);		//��ʾ�趨�¶�ֵ
					if((Temp<(Set_temp1-5))&&(Temp>0))//С���趨�¶�5�ȿ�ʼ����
					{
						BED = ON;
					}else if((Temp>Set_temp1)||(Temp==0)||(Temp<0))//�����趨�¶�ֹͣ����
					{
						BED = OFF;
					}
				}else if(MODE == ModeB)//ģʽB�¿�
				{
					LCD_Write_String(15,0,"B");//LCD��ʾ��ǰģʽ
					LCD_TEM(11,1,Set_temp2);		//��ʾ�趨�¶�ֵ
					if((Temp<(Set_temp2-5))&&(Temp>0))//С���趨�¶�5�ȿ�ʼ����
					{
						BED = ON;
					}else if((Temp>Set_temp2)||(Temp==0)||(Temp<0))//�����趨�¶�ֹͣ����
					{
						BED = OFF;
					}
				}
			}
			if(Temp<0)//��СֵΪ0
			{Temp=0;}
			LCD_V(4,0,1);//��ʾϵͳ�����ѹ
			
			LCD_TEM(4,1,Temp);				//��ʾ��ǰ�¶�ֵ
			/**********************��Ҫ���룬�޸Ŀ����������д��EEPROM����оƬ���***************************/
			////�ȶ�ȡ������֤��Ȼ���ٽ���д�룬�������ظ�д��
			if(Dat_Save_flag==1)
			{
				Flash_date1 = Get_Temp_set1();//��ȡ����
				Flash_date2 = Get_Temp_set2();//��ȡ����
				Flash_date3 = Get_Mode();//��ȡ����
				
				if((Flash_date1 != Set_temp1)||(Flash_date2 != Set_temp2)||(Flash_date3 != MODE))//���������һ���ģ�������,����ı���Ŀ���¶��򽫵�ǰ����д��Flash
				{
					Data_preservation(Set_temp1,Set_temp2,MODE);//��������
					printf("save success 1:%d,2:%d,3:%d \r\n",Set_temp1,Set_temp2,MODE);
				}
				Dat_Save_flag=0;
			}
			/************************************************/
//		printf("Set: %d \r\n",Set_temp);//���������ǰ�¶ȡ�
//			printf("Set: %f \r\n",Rt);//���������ǰ��ֵ
//		printf("T: %f \r\n",Temp);//���������ǰ�¶�
			delay_ms(500);//���0.1��ˢ��һ����Ļ			
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

 void LCD_V(uint8 a,uint8 l,uint8 p)					//��ʾ��ͨ������õĵ�ѹֵ����Ҫ��P1.0���ż�TL431�����ı�׼��ѹ2.47V���ܹ���������������֤����׼ȷ�������0.02V����
 {
	LCD_Write_Char(a,l,(int)(Filtering_Voltage(p)*6)/10+0x30);     //��λ
	LCD_Write_Char(a+1,l,(int)(Filtering_Voltage(p)*6)%10+0x30);     //��λ
	LCD_Write_Char(a+2,l,'.');
	LCD_Write_Char(a+3,l,(int)(Filtering_Voltage(p)*600)%100/10+0x30); //ʮ��λ
	LCD_Write_Char(a+4,l,(int)(Filtering_Voltage(p)*600)%100%10+0x30); //�ٷ�λ
 }
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
}
//
