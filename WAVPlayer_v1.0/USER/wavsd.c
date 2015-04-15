
/* Includes ------------------------------------------------------------------*/
#include "wavsd.h"

/* Private variables ---------------------------------------------------------*/
int res;									//��д�ļ��ķ���ֵ
//int a;

//FATFS fs2;									//��¼�ļ�ϵͳ�̷���Ϣ�Ľṹ��
FIL fdst;								//�ļ�ϵͳ�ṹ�壬�����ļ�ָ��ȳ�Ա

UINT br;									//File R/W count
BYTE *Buff, header[44];						//File copy buffer
Buffer Buff_1, Buff_2;						//˫���棡

/* Global variable */
uint32_t DataAddress;						//���ݵ�ַλ
uint32_t address;							//������ݵ�ַλ
WavHeader Nowwavheader;
uint32_t DACdone;

uint32_t volme;								//��������
uint32_t PlayOrWait;						//����/��ͣ

extern int Chose;
extern int Number;
extern uint32_t PlayIRQBuffer;

/* Init wav file which from SD ------------------------------------------------*/
void WavPlayInit(WavHeader *wavheader)
{
	uint32_t Addr = 0;
	volme = 3;

	//RIFF
	SerialPutString("\n");
	SerialPutString("\n");
	WAV_Info((*wavheader).riffheader.szRiffID, Addr, Num_4);		//'RIFF'
	(*wavheader).riffheader.szRiffID[4] = '\0';												//���һλ��ֵΪ'\0'
	Addr = Addr + Num_4 * Bit_1;															//��һ��ַ

	(*wavheader).riffheader.dwRiffSize = WAV_Num(Addr, Bit_4);		//�ļ�����
	Addr = Addr + Num_1 * Bit_4;															//��һ��ַ

	WAV_Info((*wavheader).riffheader.szRiffFormat, Addr, Num_4);	//'WAVE'
	(*wavheader).riffheader.szRiffFormat[4] = '\0';											//���һλ��ֵΪ'\0'
	Addr = Addr + Num_4 * Bit_1;															//��һ��ַ

	//FMT
	WAV_Info((*wavheader).pcmwaveformat.szFmtID, Addr, Num_4);		//'fmt' 
	(*wavheader).pcmwaveformat.szFmtID[4] = '\0';											//���һλ��ֵΪ'\0'
	Addr = Addr + Num_4 * Bit_1;															//��һ��ַ

	(*wavheader).pcmwaveformat.wBitsPerSample = WAV_Num(Addr, Bit_4);//fmt��С
	Addr = Addr + Num_1 * Bit_4;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.wFormatag = WAV_Num(Addr, Bit_2);	//��ʽ����
	Addr = Addr + Num_1 * Bit_2;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.wChannls = WAV_Num(Addr, Bit_2);	//ͨ����
	Addr = Addr + Num_1 * Bit_2;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.nSamplesPerSec = WAV_Num(Addr, Bit_4);//����Ƶ��
	Addr = Addr + Num_1 * Bit_4;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.dwAvgBytesperSec = WAV_Num(Addr, Bit_4);//��������
	Addr = Addr + Num_1 * Bit_4;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.wBlockAlign = WAV_Num(Addr, Bit_2);//DATA���ݿ鳤
	Addr = Addr + Num_1 * Bit_2;															//��һ��ַ

	(*wavheader).pcmwaveformat.wf.wBitsPerSample = WAV_Num(Addr, Bit_2);//PCMλ��
	Addr = Addr + Num_1 * Bit_2;															//��һ��ַ
	
	//DATA
	WAV_Info((*wavheader).datablack.szDataID, Addr, Num_4);			//'data'
	(*wavheader).datablack.szDataID[4] = '\0';												//���һλ��ֵΪ'\0'
	Addr = Addr + Num_4 * Bit_1;															//��һ��ַ
	
	(*wavheader).datablack.dwDataSize = WAV_Num(Addr, Bit_4);		//�������ݳ���
	Addr = Addr + Num_1 * Bit_4;															//��һ��ַ
	
	Nowwavheader = *wavheader;
	//PrintWavHeader(wavheader);
}

void PrintWavHeader(WavHeader Wavheader)
{
	char string[NumStrMax];
	SerialPutString("~~Read Start~~ ");
	
	SerialPutString("\n szRiffID: ");//1
	SerialPutString(Wavheader.riffheader.szRiffID);
	
	SerialPutString("\n dwRiffSize: ");//2
	sprintf(string, "%d", Wavheader.riffheader.dwRiffSize);
	SerialPutString(string);
	
	SerialPutString("\n szRiffFormat: ");//3
	SerialPutString(Wavheader.riffheader.szRiffFormat);
	
	SerialPutString("\n szFmtID: ");//4
	SerialPutString(Wavheader.pcmwaveformat.szFmtID);
	
	SerialPutString("\n wBitsPerSample: ");//5
	sprintf(string, "%d", Wavheader.pcmwaveformat.wBitsPerSample);
	SerialPutString(string);
	
	SerialPutString("\n wFormatag: ");//6
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wFormatag);
	SerialPutString(string);
	
	SerialPutString("\n wChannls: ");//7
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wChannls);
	SerialPutString(string);
	
	SerialPutString("\n nSamplesPerSec: ");//8
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.nSamplesPerSec);
	SerialPutString(string);
	
	SerialPutString("\n dwAvgBytesperSec: ");//9
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.dwAvgBytesperSec);
	SerialPutString(string);
	
	SerialPutString("\n wBlockAlign: ");//10
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wBlockAlign);
	SerialPutString(string);
	
	SerialPutString("\n wBitsPerSample: ");//11
	sprintf(string, "%d", Wavheader.pcmwaveformat.wf.wBitsPerSample);
	SerialPutString(string);
	
	SerialPutString("\n szDataID: ");//12
	SerialPutString(Wavheader.datablack.szDataID);
	
	SerialPutString("\n dwDataSize: ");//13
	sprintf(string, "%d", Wavheader.datablack.dwDataSize);
	SerialPutString(string);
	
}

/* Header of WAV_Info ------------------------------------------------------------------*/
void WAV_Info(uint8_t *Info, uint32_t Addr, uint32_t Num)
{
	uint32_t StartAddr, EndAddr, NowAddr, count;
	
	StartAddr = Addr;
	EndAddr = StartAddr + 1 * Num;
	NowAddr = StartAddr;
	
	for(count=0 ;NowAddr < EndAddr ;count++)
	{
		Info[count] = header[NowAddr];
		NowAddr ++;
	}
	
}

/* Header of WAV_Num -------------------------------------------------------------------*/
uint32_t WAV_Num(uint32_t Addr, uint32_t Byte)
{
	uint32_t StartAddr, NowAddr, data;
	
	StartAddr = Addr;
	NowAddr = StartAddr;
	
	if(Byte == 2)
	{
		data = (header[NowAddr+1]<<8) | header[NowAddr];
	}
	else if(Byte == 4)
	{
		data = (header[NowAddr+3]<<24) | (header[NowAddr+2]<<16) | (header[NowAddr+1]<<8) | header[NowAddr];
	}
	
	return data;
}

/* Time3 init --------------------------------------------------------------------------------*/
void Time_Init(WavHeader wavheader)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	uint32_t nSamplesPerSec = wavheader.pcmwaveformat.wf.nSamplesPerSec;
	
	//PrintWavHeader(wavheader);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);									//APB1��Ƶ8MHz
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = 1000000/nSamplesPerSec ;								//��ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 71;       										//Ԥ��Ƶֵ=(72M/1M)-1
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    										//ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  							//����ģʽ
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	//��ʱ����ʼ�����
	
	/* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the usart1 receive interrupt */
	
}



/* Play music ----------------------------------------------------------------*/
void WavPlay(WavHeader *wavheader, char *Apath)
{	
	char string[NumStrMax];
	uint32_t i;

	/**************************************/
	NVIC_Configuration();
	
	//SerialPutString(Apath);
	res = f_open(&fdst, Apath, FA_OPEN_EXISTING | FA_READ);
	//���ļ�
	br = 1;
	//a = 0;

	/* Init */
	res = f_read(&fdst, header, sizeof(header), &br);

	WavPlayInit(wavheader);
	//PrintWavHeader(*wavheader);
	

	DataAddress = 0;
	address = 0;
	

	for(i=0; i<BuffMax; i++)
	{
		Buff_1.Data[i] = 0;
		Buff_2.Data[i] = 0;
	}	
	DACdone = 0;
	Buff_1.Read = Noset;
	Buff_2.Read = Noset;

	/* Play */
	//SerialPutString("~1~\n");
	res = f_read(&fdst, Buff_1.Data, sizeof(Buff_1.Data), &br);		//���ļ���������ݶ���������1
	Buff_1.Read = Set;
	//SerialPutString("~2~\n");
	res = f_read(&fdst, Buff_2.Data, sizeof(Buff_2.Data), &br);		//���ļ���������ݶ���������2
	Buff_2.Read = Set;
	//SerialPutString("~3~\n");
	Buff_1.Read = Setting;
	Buff = Buff_1.Data;

	if(res == FR_OK)
	{
		PlayOrWait = Set;										//��ʼ����
		DAC1_Init();
		Time_Init(*wavheader);									//��ʱ����ʼ��
		
		for(;DataAddress < Nowwavheader.datablack.dwDataSize;)
		{	
			//while(DACdone == 0);
			if(DACdone == 1)							//һ���ص�������ͨ��DAC���꣬����һ���ض�ȡ
			{

				if(Buff_1.Read == Set)						//��buffer1��δ�����ݣ���buffer1����
				{
					Buff_1.Read = Setting;
					Buff = Buff_1.Data;
					Buff_2.Read = Noset;
				}
				else if(Buff_2.Read == Set)					//��buffer2��δ�����ݣ���buffer2����
				{
					Buff_2.Read = Setting;
					Buff = Buff_2.Data;
					Buff_1.Read = Noset;
				}
				else
				{
					SerialPutString("DACdone error!\n");
					while(1);
				}
				DACdone = 0;							//��ǹ��
			}
			else if(DACdone == 0)									//��䵯ҩ
			{
				if(Buff_1.Read == Noset)
				{
					while(1)
					{
						res = f_read(&fdst, Buff_1.Data, sizeof(Buff_1.Data), &br);	//���ļ���������ݶ���������
						if(res == FR_OK) break;
						else
						{
							sprintf(string, "Buff_1: %d\n", res);					//����SD��������
							//SerialPutString(string);
						}
					}
					Buff_1.Read = Set;
				}
				else if(Buff_2.Read == Noset)
				{
					while(1)
					{
						res = f_read(&fdst, Buff_2.Data, sizeof(Buff_2.Data), &br);	
						if(res == FR_OK) break;
						else
						{
							sprintf(string, "Buff_2: %d\n", res);					//����SD��������
							//SerialPutString(string);
						}
					}
					
					Buff_2.Read = Set;
					//SerialPutString("\nRead Buff_2!\n");
				}
			}
			else if(DACdone == 2)											//�������ţ�����ѭ��
			{
				break;
			}
		}
		
		for(i=0; i<BuffMax; i++)											//˫��������
		{
			Buff_1.Data[i] = 0;
			Buff_2.Data[i] = 0;
		}
		//SerialPutString("\nRead Ok!\n");
		f_close(&fdst);
		
	}
}
	
/* ��ʱ��3�жϴ�����������DAC��Ƶ��� */
// WavHeader wavheader
void TIM3_IRQHandler()
{
	uint16_t Data, data;

//	char string[NumStrMax];

	if (DataAddress < Nowwavheader.datablack.dwDataSize)					//ֻѰַ����
	{
		if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
		{
			TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
			if(Nowwavheader.pcmwaveformat.wf.wChannls == 1)
			{
				if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 8)		//��������λ
				{
					Data = Buff[address];

					DAC_SetChannel1Data(DAC_Align_12b_R, Data/volme);
					DAC_SetChannel2Data(DAC_Align_12b_R, Data/volme);
					
					DataAddress = DataAddress + Bit_1;
					address = address + Bit_1;
				}
				else if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 16)	//������ʮ��λ
				{
					Data = (Buff[address+1]<<8) | Buff[address];
					
					data = Data+(1<<15) ;
					data = data>>4;
					
					DAC_SetChannel1Data(DAC_Align_12b_L, data/volme);
					DAC_SetChannel2Data(DAC_Align_12b_L, data/volme);

					DataAddress = DataAddress + Bit_2;
					address = address + Bit_2;
				}
				else
				{
					SerialPutString("wBitsPerSample error!");
					while(1)
					{}
				}
			}
			else if(Nowwavheader.pcmwaveformat.wf.wChannls == 2)
			{
				if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 8)		//��������λ
				{
					Data = Buff[address];				
					DAC_SetChannel1Data(DAC_Align_12b_R, Data/volme);
					DataAddress = DataAddress + Bit_1;
					address = address + Bit_1;
					
					Data = Buff[address];
					DAC_SetChannel2Data(DAC_Align_12b_R, Data/volme);
					DataAddress = DataAddress + Bit_1;
					address = address + Bit_1;
				}
				else if(Nowwavheader.pcmwaveformat.wf.wBitsPerSample == 16)	//������ʮ��λ
				{
					Data = (Buff[address+1]<<8) | Buff[address];
					data = Data+(1<<15) ;
					data = data>>4;
					DAC_SetChannel1Data(DAC_Align_12b_L, data/volme);
					DataAddress = DataAddress + Bit_2;
					address = address + Bit_2;
					
					Data = (Buff[address+1]<<8) | Buff[address];
					data = Data+(1<<15) ;
					data = data>>4;
					DAC_SetChannel2Data(DAC_Align_12b_L, data/volme);
					DataAddress = DataAddress + Bit_2;
					address = address + Bit_2;
				}
				else
				{
					SerialPutString("wBitsPerSample error!");
					while(1)
					{}
				}
			}
			else
			{
				SerialPutString("wChannls error!");
				while(1)
				{}
			}
		}

		if(address >= BuffMax)
		{
//			if(Buff_1.Read == Setting) 
//				Buff_1.Read = Setted;
//			else if(Buff_2.Read == Setting)
//				Buff_2.Read = Setted;
//			else
//			{}
			DACdone = 1;
			address = 0;
		}		

	}

	else if (DataAddress >= Nowwavheader.datablack.dwDataSize)
	{
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		SerialPutString("Play end\n");
		DACdone = 2;
		address = 0;		
	}
}

/* �����жϴ����� --------------------------------------- */
/* �������� */
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		//SerialPutString("volme--\n");
		if (volme < 8)
		{
			volme++;
			SerialPutString("���� -\n");
			
		}			
		EXTI_ClearITPendingBit(EXTI_Line3);
	}	
}


void EXTI15_10_IRQHandler(void)
{	
	char string[NumStrMax];
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		//SerialPutString("volme++\n");
		if (volme > 1)
		{
			volme--;
			SerialPutString("���� +\n");
			
		}	
		EXTI_ClearITPendingBit(EXTI_Line15);		
	}	
	
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
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
		sprintf(string, "%d\n" ,Chose);
		SerialPutString(string);
		SerialPutString("��һ��\n");
		/* ������һ�� */
		PlayIRQBuffer = Set; 
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
		
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
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
		sprintf(string, "%d\n" ,Chose);
		SerialPutString(string);
		SerialPutString("��һ��\n");
		/* ������һ�� */
		PlayIRQBuffer = Set; 
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

/* ����/��ͣ/ֹͣ ------------------------------------------------- */
void EXTI9_5_IRQHandler(void)
{
	if((EXTI_GetITStatus(EXTI_Line7) != RESET) && (DACdone != 2))
	{
		//SerialPutString("key\n");
		if(PlayOrWait == Set)
		{
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
			PlayOrWait = Noset;
			SerialPutString("��ͣ\n");
		}
		else if(PlayOrWait == Noset)
		{
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			PlayOrWait = Set;
			SerialPutString("��������\n");
		}
		
		EXTI_ClearITPendingBit(EXTI_Line7);
		
	}
	
	if((EXTI_GetITStatus(EXTI_Line8) != RESET) && (DACdone != 2))
	{
		SerialPutString("ֹͣ\n");
		DACdone = 2;
		PlayIRQBuffer = Noset;
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

