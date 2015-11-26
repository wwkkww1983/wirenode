#include "ACom.h"
#include "stdafx.h"
extern HANDLE A_hCom;
#ifndef _ACOM_H_
#define _ACOM_H_
int OpenComm(char* port)
{
	CString _port = CString(port);
	LPCWSTR wszClassName = new WCHAR[_port.GetLength() + 1];
	wcscpy((LPTSTR)wszClassName, T2W((LPTSTR)_port.GetBuffer(NULL)));
	_port.ReleaseBuffer();
	/*ʵ��char*��LPCWSTR��ת��*/
	A_hCom = CreateFile(wszClassName,
		GENERIC_READ|GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if(A_hCom == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		printf("opencomm failed %d\n",dwErr);
		return FALSE;
	}
	else
	{
		// �򿪳ɹ�����ʼ������
		DCB wdcb = {0};
		GetCommState(A_hCom, &wdcb); //��ȡ��ǰ���ڲ���
		wdcb.BaudRate = CBR_9600;         // ������
		wdcb.ByteSize = 8;                  // ����λ8
		wdcb.fBinary  = TRUE;				// �����Ʒ�ʽ
		wdcb.fParity  = FALSE;
		wdcb.Parity   = NOPARITY;			// ����żУ��
		wdcb.StopBits = ONESTOPBIT;        //1ֹͣλ
		//	wdcb.fRtsControl = false;
		//	wdcb.fDtrControl = false;
		//	wdcb.fOutxCtsFlow = false;
		//	wdcb.fOutxDsrFlow = false;
		wdcb.XonLim = 2048;
		wdcb.XoffLim = 512;
		wdcb.EofChar = 0;
		// ���ô��ڲ���
		SetCommState(A_hCom, &wdcb);

		// ���ô��ڳ�ʱ����
		COMMTIMEOUTS to =                   // ���ڳ�ʱ���Ʋ���
		{
			MAXDWORD,                       // ���ַ������ʱʱ��
			10,                              // ������ʱÿ�ַ���ʱ��
			10,                              // �����ģ�����ģ�����ʱʱ��
			MAXDWORD,                       // д����ʱÿ�ַ���ʱ��
			10                               // �����ģ�����ģ�д��ʱʱ��
		};
		SetCommTimeouts(A_hCom, &to);
		// ���ô��ڻ������
		SetupComm(A_hCom, 1024, 1024);
		// ��ղ��������ڵ�ǰ����
		PurgeComm(A_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
		printf("open com done\n");
		//m_hCom = hCom;						// ������
		return 1;
	}
	return 0;
}
int ReadData(HANDLE hCom,unsigned char* pBuffer, unsigned char ulen)
{	
	// �Ӵ��ڶ�ȡ����
	DWORD dwRead;
	if (!ReadFile(hCom, pBuffer, ulen, &dwRead, NULL))
	{
		DWORD dwErr = GetLastError();
		//printf("%ld\n",dwErr);
		printf("read error %ld\n",dwErr);
		return dwErr;
	}
	//	printf("read done %d\n",dwRead);
	//	PurgeComm(A_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
	return 0;
}

int WriteData(HANDLE hCom, unsigned char *pBuffer, unsigned char uLen)
{	
	// д�����ݵ�����
	DWORD dwWritten;
	if (uLen > 0)
	{
		dwWritten = 0;
		if (!WriteFile(hCom, pBuffer, uLen, &dwWritten, NULL))
		{
			DWORD dwErr = GetLastError();
			printf("wirte error %ld\n",dwErr);
			return dwErr;
		}
		else{
			//			printf("write done %d\n",dwWritten);
		}
	}
	//	PurgeComm(A_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
	return 0;
}
void CloseComm(HANDLE hCom)
{		
	CloseHandle(hCom);
	hCom = NULL;
}
//int memset2(HANDLE hCom, Data data){
//	return 1;//��ȡһ���ɹ�
//	return 0;// û������
//}
/****У��ͼ��㺯��********/
unsigned char  CheckSum(unsigned char *uBuff, unsigned char uBuffLen) 
{
	unsigned char i,uSum=0;
	for(i=3;i<uBuffLen;i++)
	{
		uSum = uSum + uBuff[i];
	}
	uSum = (~uSum) + 1;
	if(uSum > 0xf0)
		uSum -= 16;
	//	printf("%X\n",uSum);
	return uSum;
}
#endif
