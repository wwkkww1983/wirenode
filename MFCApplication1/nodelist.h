#pragma once
#include <stdlib.h> /*��ma l l o c ( ) ��ͷ�ļ�*/  
#include <stdio.h> 

typedef struct Data{
	int ex;
	int ey;
	int nlist;
}Data;
struct Node{
	Data data;
	struct Node *next;
};

class nodelist
{
public:
	nodelist();
	~nodelist();
public:
	int add(Data data);
	int del(Node *node);
	int del(Data data);
	Data pushnext();
	Node* search(Data data);
	int delall();
	
private:
	Node *head;
	Node *current;
	int n = 0;
};

