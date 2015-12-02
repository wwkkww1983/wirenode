#pragma once
#include <stdlib.h> /*��ma l l o c ( ) ��ͷ�ļ�*/  
#include <stdio.h> 
/*ʵ��������Ļ����������д�����
NL��*/

typedef struct Data{
	double ex;
	double ey;
	int nlist;
	double rssi;
}Data;
struct Node{
	Data data;
	struct Node *next;
};

class NL
{
public:
	NL();
	~NL();
public:
	int add(Data data);
	int del(Node *node);
	int del(Data data);
	Data pushnext();
	Node* search(Data data);
	Node* search(int nlist);
	int delall();
	
private:
	Node *head;
	Node *current;
	int n = 0;
};

