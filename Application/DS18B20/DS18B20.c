#include <reg52.h>
#include <intrins.h>
#include "DS18B20.h"
#include "Sys.h"//�Զ�����ʱ������uint8��uint16
/***************************************************
*								DS18B20��λ�Ӻ���
*						 ��������550us���ͷ�66us
****************************************************/
void ds18b20_reset(void)
{
	bit flag=1;
	while (flag)				//�жϳ�ʼ���Ƿ�ɹ�
	{
		while (flag)			//�ж�DS18B20�Ƿ��������
	 	{
 			DQ = 1;
			delay_11us(1);	//�ߵ�ƽԼ11us
 			DQ = 0;
 			delay_11us(50); //�͵�ƽ550us
 			DQ = 1; 
 			delay_11us(6);  //�ͷ�66us
 			flag = DQ;			//����״̬����
   	}
		delay_11us(50);   //��ʱ550us
		flag = ~DQ;				//����״̬����
	}
	DQ=1;
}
/***************************************************
*							������д��һ���ֽ�
****************************************************/
void write_byte(uint8 val)
{
	uint8 i;
	for (i=0; i<8; i++)
	{
		DQ = 1;								//дʱ϶׼������
		_nop_();
		DQ = 0;								//����DS18B20��׼������
		nops(); 							//4us
		DQ = val & 0x01;      //���λ�Ƴ�
		delay_11us(6);        //66us
		val >>= 1;          	//����һλ
	}
	DQ = 1;
	delay_11us(1);  
}
/***************************************************
*					    	�����߶���һ���ֽ�
****************************************************/
uint8 read_byte(void)
{
	uint8 i, value=0;
	for (i=0; i<8; i++)
	{
		DQ=1;								//��ʱ϶׼������
		_nop_();
		value >>= 1;
		DQ = 0;							//����DS18B20��׼������
		nops();   					//4us
		DQ = 1;							//�ͷ�����
		nops();   					//4us 
		if (DQ)							//0��1���ж�
			value|=0x80;
		delay_11us(6);           //66us
	}
	DQ=1;
	return(value);				//���زɼ���������
}
/***************************************************
*								�¶�ת���Ӻ���
*						���ݲ���˳�򣬷���ת������
****************************************************/
void start_convert(void)
{
	ds18b20_reset();
	write_byte(0xCC); 		// ��Skip ROM����
	write_byte(0x44); 		// ��ת������
}
/***************************************************
*								�¶ȶ�ȡ�Ӻ���
*						���ݲ���˳�򣬶�ȡ�¶�ֵ
****************************************************/
uint16 read_temp(void)
{
	uint8 temp_data[2]; 			// �����¶��ݷ�
	uint16 temp;

	ds18b20_reset();  				// ��λ
	write_byte(0xCC); 				// ��Skip ROM����
	write_byte(0xBE); 				// ��������
	temp_data[0]=read_byte();  //�¶ȵ�8λ
	temp_data[1]=read_byte();  //�¶ȸ�8λ
	temp = temp_data[1];
	temp <<= 8;
	temp |= temp_data[0];
	return temp;
}
float Get_T(void)
{
	float T;
	uint16 Tem;
	start_convert();//��ʼ�ɼ��¶�����
	//delay_ms(100);
	Tem = read_temp();
	if(Tem>60000)//�¶�Ϊ��
	{
		Tem = ~Tem;
		Tem++;
		T = -(Tem*0.0625);
		return T;
	}
	else
	{T = Tem*0.0625;return T;}
}
