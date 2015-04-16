
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/

uint32_t VolumeSub;
uint32_t VolumeAdd;
uint32_t NextMusic;
uint32_t LastMusic;
uint32_t PlayWait;
uint32_t StopKey;

extern uint32_t volume;
extern int Chose;
extern int Number;
extern int DACdone;
extern int PlayIRQBuffer;
extern int PlayOrWait;					

/* ������ ------------------------------------------------------------------- */
void Volume_Sub(void)
{
	int i;

	if (volume < 8)
	{
		volume++;
		SerialPutString("���� -\n");	
	}		
	for(i=0; i<200 ;i++);	
	
	VolumeSub = Noset;
		
}

/* ������ ------------------------------------------------------------------- */
void Volume_Add(void)
{
	int i;
	
	if (volume > 1)
	{
		volume--;
		SerialPutString("���� +\n");
		
	}	
	for(i=0; i<100 ;i++);
	
	VolumeAdd = Noset;
	
}

/* ��һ�� ------------------------------------------------------------------- */
void Next_Music(void)
{
	int i;
	
	/* ֹͣ��ǰ���� */	
	DACdone = 2;
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		
	if(Chose < (Number-1))
	{
		Chose++;
	}
	else if(Chose == (Number-1))					//���������б������һ�׸裬���ص�һ�׸�
	{
		SerialPutString("�ѵ����һ�ף�����ת����һ�׸�\n");
		Chose = 0;
	}
	else
	{
		SerialPutString("Chose Error!\n");
	}
//	sprintf(string, "%d\n" ,Chose);
//	SerialPutString(string);
	SerialPutString("��һ��\n");
	/* ������һ�� */
	PlayIRQBuffer = Set; 
	for(i=0; i<100 ;i++);
	
	NextMusic = Noset;
	
}

/* ��һ�� ---------------------------------------------------------- */
void Last_Music(void)
{
	int i;
		
	/* ֹͣ��ǰ���� */
	DACdone = 2;
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	
	if(Chose > 0)
	{
		Chose--;
	}
	else if(Chose == 0)
	{
		SerialPutString("�ѵ���һ�ף�����ת�����һ�׸�\n");
		Chose = Number - 1;
	}
//	sprintf(string, "%d\n" ,Chose);
//	SerialPutString(string);
	SerialPutString("��һ��\n");
	/* ������һ�� */
	PlayIRQBuffer = Set; 
	for(i=0; i<100 ;i++);
	
	LastMusic = Noset;
}

/* ��ͣ/�������� -------------------------------------------------------- */
void Play_Wait(void)
{
	int i;
	if((PlayOrWait == Set) && (DACdone != 2))
	{
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		PlayOrWait = Noset;
		SerialPutString("��ͣ\n");
	}
	else if((PlayOrWait == Noset) && (DACdone != 2))
	{
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		PlayOrWait = Set;
		SerialPutString("��������\n");
	}
	for(i=0; i<100 ;i++);
	
	PlayWait = Noset;
}

/* ֹͣ���� --------------------------------------------------------------- */
void Stop_Key(void)
{
	int i;
	if(DACdone != 2)
	{
		SerialPutString("ֹͣ\n");
		DACdone = 2;
		PlayIRQBuffer = Noset;
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		for(i=0; i<100 ;i++);
	}
	
	StopKey = Noset;
}
