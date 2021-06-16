#include "STC12C5A60S2.h"
#include "intrins.h"
#include "STC12(ADC flag).h"
#include <math.h>
float temp;
float R1=23.84,R2=23.65,R3=24.05,R4=23.65,R5=23.7,R0_1,R0_2,R0_3,R0_4;
//int c;           
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
		float sv=0.895;//2.465
		V0=(1024*sv)/(float)Get_ADC_Result(0);//���ݱ�׼�ο���ѹ����оƬ�Ĺ�����ѹ��ϵͳ�Ĳο���ѹ��
		return V0;
	}
	//************************************ADC���ݼ��㺯��
float Count(uchar ch)
{
	temp=(float)Get_ADC_Result(ch)*Operating_Voltage()/1024; //4.85Ϊ�ο���ѹ��1024�����š���10λAD
	return temp;
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

float Res_Calculation()//���ݵ����ѹԭ������������ĵ���ֵ
{
	float R0,R;
		R0_1=R1/((Filtering_Voltage(1)-Filtering_Voltage(2))/Filtering_Voltage(5));
		R0_2=R2/((Filtering_Voltage(2)-Filtering_Voltage(3))/Filtering_Voltage(5));
		R0_3=R3/((Filtering_Voltage(3)-Filtering_Voltage(4))/Filtering_Voltage(5));
		R0_4=R4/((Filtering_Voltage(4)-Filtering_Voltage(5))/Filtering_Voltage(5));
		
		R0=(R0_1+R0_2+R0_3+R0_4)/4;//��ƽ��ֵ�ó����յĲ�õ�RO��R5�뵼�岢��֮��ĵ��裩
		
	R=((R5*R0)/(R5-R0))-1.07;//��ò�����ֵ,,//1.08Ϊ����ֵ
	if (R>2000)
		return 0;
	else
		return R;
}

float p_Calculation()
{
	float p;
	if(Res_Calculation()==0)
	{return 0;}
	else
	{
		p=(Res_Calculation()*R_S)*1000/R_L;//�����������ĵ����ʵ�λΪ m��/m
	  return p;
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////
