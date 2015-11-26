#ifndef ACOM_H
#define ACOM_H

#include "windows.h"
#include <process.h>
#include "string.h"
#include "nodelist.h"

/***************
���ڹ��ܺ���:
��opencomm
��ȡReadData
д��WriteData
�ر�CloseComm
***************/


int OpenComm(char* port);

int ReadData(HANDLE hCom,unsigned char* pBuffer, unsigned char ulen);

int WriteData(HANDLE hCom, unsigned char *pBuffer, unsigned char uLen);	

void CloseComm(HANDLE hCom);

//int memset2(HANDLE hCom,Data data);

/****У��ͼ��㺯��********/
unsigned char  CheckSum(unsigned char *uBuff, unsigned char uBuffLen); 


HANDLE A_hCom;//ȫ�־��



#endif