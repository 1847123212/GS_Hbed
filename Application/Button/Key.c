#include <reg52.h>
#include "key.h"
#include "Sys.h"


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3����
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3...!!
uint8 Key_Scan(uint8 mode)//�������
{
	static uint8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(Key1==0||Key2==0||Key3==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(Key1==0)return 1;
		else if(Key2==0)return 2;
		else if(Key3==0)return 3;
	}else if(Key1==1&&Key2==1&&Key3==1)key_up=1;
 	return 0;// �ް�������
}

