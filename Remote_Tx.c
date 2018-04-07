/*
By WMD
��׼NEC ����ң�ط�������
��Ҫ������ʱ�� ����һ����������38Khz�ز� ����һ�����ھ�׼�������
*/
#include "Remote_Tx.h"
#include "tim.h"
#define REMOTE_TIM htim4//ң��pwm�����õĶ�ʱ�� Ҫ��:��Ƶ��ÿ1us���� pwmƵ��Ϊ38Khz ռ�ձ�Ϊ1/3
#define REMOTE_CH TIM_CHANNEL_4
#define US_TIM htim3 //����΢����ʱ�Ķ�ʱ�� Ҫ�� ��Ƶ��ÿ1us���� ARR>10000
#define RE_H __HAL_TIM_SET_COMPARE(&REMOTE_TIM,REMOTE_CH,9)//�����߼�1
#define RE_L __HAL_TIM_SET_COMPARE(&REMOTE_TIM,REMOTE_CH,0)//�����߼�0
void Remote_Init()
{
	HAL_TIM_Base_Start(&US_TIM);
	HAL_TIM_PWM_Start(&REMOTE_TIM,REMOTE_CH);
	RE_L;
}
static void Create_Bit(uint8_t bit)
{
	US_TIM.Instance->CNT=0;//��ռ�����
	RE_H;
	while(US_TIM.Instance->CNT<560);
	RE_L;
	if(bit)
	{
		while(US_TIM.Instance->CNT<1680+560);
	}else{
		while(US_TIM.Instance->CNT<560+560);
	}
}
static void Create_invoke(void)//����������
{
	US_TIM.Instance->CNT=0;//��ռ�����
	RE_H;
	while(US_TIM.Instance->CNT<9000);
	RE_L;
	while(US_TIM.Instance->CNT<9000+4500);
}
static void Create_stop(void)//����ֹͣλ
{
	US_TIM.Instance->CNT=0;//��ռ�����
	RE_H;
	while(US_TIM.Instance->CNT<300);
	RE_L;
	while(US_TIM.Instance->CNT<600);
}
void Remote_Transmit(uint8_t value,uint8_t addr)
{
	uint32_t Send_code=(uint8_t)(~addr) + ((uint8_t)(addr)<<8) + ((uint8_t)(~value)<<16) + ((uint8_t)(value)<<24);
	int8_t Send_index;
	Create_invoke();
	for(Send_index=31;Send_index>=0;Send_index--)//��λ�����ز�
	{
		Create_Bit((Send_code & (1<<Send_index))!=0);
	}
	Create_stop();
}
