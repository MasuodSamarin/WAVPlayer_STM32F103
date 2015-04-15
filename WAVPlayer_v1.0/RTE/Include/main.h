/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define SerialPutString(x)   Serial_PutString((uint8_t*)(x))
#define NumStrMax					64
#define NumFileMax					64

/* Private define ------------------------------------------------------------*/
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      32  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define SD_OPERATION_ERASE          0
#define SD_OPERATION_BLOCK          1
#define SD_OPERATION_MULTI_BLOCK    2 
#define SD_OPERATION_END            3


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	int ID;								//���ű��
	char Name[NumStrMax];				//�ļ���
	char Apath[NumStrMax];				//����·��
	long int Size;						//�ļ���С
}WavFile;								//�ļ���Ϣ

typedef struct
{
	WavFile File[NumFileMax];
	int Number;							//�����б�������Ŀ
}PlayList;								//�����б�


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART1_Init(void);

void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);

void DAC1_Init(void);
void DAC_Play(void);

void Flash_Init(void);
void Flash_Erase(void);
void Flash_Write(void);
void Flash_Read(void);
void Flash_Write_String(uint8_t *s);
void Flash_Write_Char(uint8_t c);
void Flash_Read_Choose(uint32_t StartAddr, uint32_t Num, uint32_t BitNum);
uint16_t Flash_Read_Wav_Data(uint32_t StartAddr, uint32_t BitNum);

void LED_Init(void);
void LED_Play(void);

void SD_Config(void);
void PrintCardType(void);

void NVIC_Configuration(void);

void ShowAllFile(PlayList *PlayList);
void PrintList(PlayList List);

void EXTI_Play_Config(void);
void EXTI_LastNext_Config(void);
void EXTI_Volume_Config(void);
void EXTI_PlayOrWait_Config(void);
void EXTI_Stop_Config(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
