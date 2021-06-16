#ifndef _STC12_ADC_FLAG_H_
#define _STC12_ADC_FLAG_H_
#define uchar unsigned char
#define uint unsigned int
#define FOSC    11059200L
#define BAUD    9600
/********************************************************************
                             ��ʼ������
*********************************************************************/
//sfr ADC_CONTR   =   0xBC;           //ADC ���ƼĴ���
//sfr ADC_RES     =   0xBD;           //ADC ת������Ĵ���
//sfr ADC_RESL    =   0xBE;           //ADC ��2λ����Ĵ���
//sfr P1ASF       =   0x9D;           //P1��ģ�⹦�ܿ��ƼĴ���
/********************************************************************
                              ADC ����ת������
*********************************************************************/
#define ADC_POWER   0x80            //ADC ��Դ����λ
#define ADC_FLAG    0x10            //ADC ת��������־λ
#define ADC_START   0x08            //ADC ��ʼת������λ
#define ADC_SPEEDLL 0x00            //540 ��ʱ������ת��һ��
//#define ADC_SPEEDL  0x20            //360 ��ʱ������ת��һ��
//#define ADC_SPEEDH  0x40            //180 ��ʱ������ת��һ��
//#define ADC_SPEEDHH 0x60            //90 ��ʱ������ת��һ��
#define COR   2.47										//P10�˿ڱ�׼�ο���ѹ
#define R_L  0.025											//������ϵĳ��ȣ���λ	M
#define R_S  0.0000785										//������ϵĺ����� ��λƽ����
//**********************************��ʱ����
extern void Delay1(uint n);
//******************************ADC��ʼ������
void Init_ADC();
//***********************************ADC����ת������
uint Get_ADC_Result(uchar ch);
//************************************ADC���ݼ��㺯��
float Operating_Voltage();
float Count(uchar ch);
float Filtering_Voltage(uchar ch);
float Res_Calculation();				//�������ֵ�����ݵ����ѹ����
float p_Calculation();					//�����������ĵ絼ֵ
#endif 