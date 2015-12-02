#pragma once
#include <stdlib.h> /*含ma l l o c ( ) 的头文件*/  
#include <stdio.h> 
/*实现了链表的基本操作，有待完善
NL类*/

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

