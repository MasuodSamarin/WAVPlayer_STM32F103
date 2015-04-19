
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ff.h"

/* Private variables ---------------------------------------------------------*/
//FATFS fs;									//��¼�ļ�ϵͳ�̷���Ϣ�Ľṹ��
FILINFO finfo;
DIR dirs;
char path[32] = {"0:/����"};				//ָ��Ŀ¼

/*----------------------------------------------------------------------------*/
void PrintList(PlayList List)
{
	char string[NumStrMax];
	int i;
	
	sprintf(string ,"\n%s path:\n", path);
	SerialPutString(string);
	sprintf(string, "There are %d files:\n", List.Number);
	for(i=0 ;i<List.Number; i++)
	{
		sprintf(string ,"ID: %d, Name: %s, Size: %ld\n" ,List.File[i].ID, List.File[i].Name, List.File[i].Size);
		SerialPutString(string);
	}
}

/* ��ȡ����Ŀ¼������ṹ���� ------------------------------------------------*/
void ShowAllFile(PlayList *PlayList)
{	
//	char string[NumStrMax];
	int res;								//��д�ļ��ķ���ֵ
	int i;
	char *Name;
	
#if _USE_LFN
	char lfn[128];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
#endif

	//֧�����ĺͳ��ļ���������������ʼ������Ҫ
	(*PlayList).Number=0;								//��ʼ�������б��е���ĿΪ0
	
	//��ӡָ���ļ�Ŀ¼
	res = f_opendir(&dirs, path);							//��һ��Ŀ¼
	if(res == FR_OK)
	{		
		
		i = 0;

		while(f_readdir(&dirs, &finfo) == FR_OK)
		{
			if(!finfo.fname[0]) 
			{
				//SerialPutString("\n~~~!~~~\n");
				break;					//�������ͷ��������
			}
			if((finfo.fattrib != AM_DIR) && (finfo.fname[0] != '.') && (finfo.lfname[0] != '.'))
			{
#if _USE_LFN
				Name = *finfo.lfname ? finfo.lfname : finfo.fname;
#else
				Name = finfo.fname;
#endif				
//				SerialPutString(Name);
//				SerialPutString("\n");
				
				(*PlayList).File[i].ID = i;
				sprintf((*PlayList).File[i].Name, "%s", Name);
				sprintf((*PlayList).File[i].Apath, "%s/%s", path, Name);
				(*PlayList).File[i].Size = finfo.fsize;
				(*PlayList).Number++;
				i++;
				
			}
		}
	}

	
}
