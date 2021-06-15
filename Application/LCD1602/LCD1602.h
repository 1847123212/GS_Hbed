#ifndef __LCD1602_H__
#define __LCD1602_H__
sbit RS = P0^5;   //����˿� �˿����ֿ������и���
sbit RW = P0^6;
sbit EN = P0^7;
#define DataPort P2

void DelayUs2x(unsigned char t);//��ʱ����
void DelayMs(unsigned char t);

bit LCD_Check_Busy(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Clear(void);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);//X:λ�ã�0~15��Y����0~1������
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);
void LCD_Init(void);
/*ʹ�÷�����eg:
	  LCD_Init();               // ��ʼ��LCD
		
		while(1)
		{
				LCD_Clear(); 
				LCD_Write_Char(3,0,(int)Count(0)+0x30);     //��λ
				LCD_Write_Char(4,0,'.');
				 
				LCD_Write_String(0,1,"hello");
		}
	
*/
#endif