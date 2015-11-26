#ifndef ACOM_H
#define ACOM_H

#include "windows.h"
#include <process.h>
#include "string.h"
#include "nodelist.h"

/***************
串口功能函数:
打开opencomm
读取ReadData
写入WriteData
关闭CloseComm
***************/


int OpenComm(char* port);

int ReadData(HANDLE hCom,unsigned char* pBuffer, unsigned char ulen);

int WriteData(HANDLE hCom, unsigned char *pBuffer, unsigned char uLen);	

void CloseComm(HANDLE hCom);

//int memset2(HANDLE hCom,Data data);

/****校验和计算函数********/
unsigned char  CheckSum(unsigned char *uBuff, unsigned char uBuffLen); 


HANDLE A_hCom;//全局句柄



#endif