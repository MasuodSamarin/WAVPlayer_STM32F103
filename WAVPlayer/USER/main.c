
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wavsd.h"
#include "wav.h"

/* Private variables ---------------------------------------------------------*/
uint32_t PlayIRQBuffer;						//����1����ֵ
int volume;

FATFS fs;									//��¼�ļ�ϵͳ�̷���Ϣ�Ľṹ��
int Chose;									//��ѡ��ID
int Number;									//��������
char *Apath;								//�����ļ��ľ���·��
char *Name;									//�����ļ���

int main(void)
{ 
	
	char string[NumStrMax];

	WavHeader wavheader;
	PlayList Playlist;

	PlayIRQBuffer = Noset;
	Chose = 0;										//Ĭ��ѡ�񲥷ŵ�һ�׸�
	volume = 3;
	
	USART1_Init();
	SerialPutString("\nUsart init!\n");

	EXTI_Play_Config();
	EXTI_LastNext_Config();
	SerialPutString("\nEXTI init!\n");
	
	f_mount(0, &fs);								//�����ļ�ϵͳ
	
	ShowAllFile(&Playlist);							//��SD���ж�ȡָ��Ŀ¼�µ��ļ��б�
	PrintList(Playlist);
	Number = Playlist.Number;

	Apath = Playlist.File[Chose].Apath;
	Name = Playlist.File[Chose].Name;
	sprintf(string ,"This music is: %s\n" ,Name);
	SerialPutString(string);
	
	while(1)										//������ѭ��
	{
		if (PlayIRQBuffer == Set)
		{	
			EXTI_Volume_Config();
			EXTI_PlayOrWait_Config();
			EXTI_Stop_Config();
			
			Apath = Playlist.File[Chose].Apath;
			Name = Playlist.File[Chose].Name;
			sprintf(string ,"This music is: %s" ,Name);
			SerialPutString(string);

			//PrintList(PlayList);
			WavPlay(&wavheader, Apath);				//�����ļ�
			
			ShowAllFile(&Playlist);	
			
		}
	}
}

/* �����жϺ��� */
void USART1_IRQHandler(void)
{
	uint8_t Key;
	char string[NumStrMax];

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		Key = USART_ReceiveData(USART1);

		SerialPutChar(Key);
		
		if ((Key >= '0')&&(Key <= (Number+'0')))
		{
			Chose = Key - '0';
			sprintf(string, "Chose = %d\n", Chose);
			SerialPutString(string);
			PlayIRQBuffer = Set; 
		}
		else 
		{
			SerialPutString("Bad value!\n");
		}
		
	}
}



