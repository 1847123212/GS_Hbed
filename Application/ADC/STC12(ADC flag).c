#include "STC12C5A60S2.h"
#include "intrins.h"
#include "STC12(ADC flag).h"
#include <math.h>
float Voltage;     
//**********************************��ʱ����(n=1: 1T 8.65ms,12T 59.9ms)
void Delay1(uint n)
{
    uint x;
    while (n--)
    {
        x = 5000;
        while (x--);
    }
}
//******************************ADC��ʼ������
void Init_ADC()
{
    P1ASF = 0x07;                      //����P1��Ϊ��©ģʽ��ʹ��AD����
    ADC_RES = 0;                       //AD���ݼĴ�����8λ���
		ADC_RESL= 0;        //AD���ݼĴ�����2λ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;  //��AD��Դ��ת������540
    Delay1(2);                        //��ʱ
}
//***********************************ADC����ת������
uint Get_ADC_Result(uchar ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch ;//��ʼת��
    _nop_();                          //��ʱһ����������
    _nop_();        //��ʱһ����������
    _nop_();        //��ʱһ����������
    _nop_();        //��ʱһ����������
    while (!(ADC_CONTR & ADC_FLAG));  //�ȴ�ת������(ADC_FLAG=0ʱһֱ�ȴ���ֱ����Ϊ1����)
    ADC_CONTR &= ~ADC_FLAG;           //�ر�ADת��
    return (ADC_RES*4+ADC_RESL);            //�������ݣ�10λADֵ��ADC_RES��8λ+ADC_RESL��2λ��
}
/////////////////////P1.0������Ǳ�׼�Ĳο���ѹ���������㶨Ϊ2.47V/////////////////
//////����Ϊϵͳ����ֵʵʱ����ϵͳ�Ĳο���ѹ��֤��������/////////////////////
float Operating_Voltage()
	{
		float V0;
		float sv=2.452;//P1.0����TL431�ο���ѹ
		V0=(1024*sv)/(float)Get_ADC_Result(0);//���ݱ�׼�ο���ѹ����оƬ�Ĺ�����ѹ��ϵͳ�Ĳο���ѹ��
		return V0;
	}
	//************************************ADC���ݼ��㺯��
float Count(uchar ch)
{
	Voltage=(float)Get_ADC_Result(ch)*Operating_Voltage()/1024; //4.85Ϊ�ο���ѹ��1024�����š���10λAD
	return Voltage;
}
///////////////////////////////��������ʮ��ȡƽ��ֵ���أ���֤�������ݵ��ȶ���/////////////////////////
float Filtering_Voltage(uchar ch)
{
	float sum=0,average;
	uchar i;
	for(i=0;i<10;i++)
	{
		sum=sum+Count(ch);
	}
	average=sum/10;
	return average;
}
