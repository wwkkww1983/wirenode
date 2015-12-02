#include "stdafx.h"
#include "nodelist.h"


nodelist::nodelist()
{
	head = NULL;
	current = NULL;
}

int nodelist::add(Data data){
	Node *p1, *p2;
	p1 = p2 = (Node*)malloc(sizeof(Node));
	p1->data = data;
	p1->next = NULL;
	if (head == NULL){
		head = p1;
	}
	else{
		p2 = head;
		while (p2->next != NULL){
			p2 = p2->next;
		}
		p2->next = p1;
	}
	current = head;
	return 1;
}

int nodelist::del(Node *node){
	if (node == NULL)//查找空节点
		return 0;
	if (head = node)
		current = head;
		head = head->next;
	return 1;
	Node* p1 = head;
	while (p1->next!=NULL&&p1->next!= node){
		p1 = p1->next;
	}
	if (p1->next == NULL){
		return -1;//未找到
	}
	else{
		p1->next = p1->next->next;
		current = head;
		return 1;
	}
}

int nodelist::del(Data data){
	if (del(search(data))){
		return 1;
	};
	return 0;
}

Node* nodelist::search(Data data){
	Node* p1 = head;
	while (p1!=NULL&&p1->data.nlist != data.nlist){
		p1 = p1->next;

	}
	if (p1 == NULL){
		return NULL;
	}
	else
	{
		return p1;
	}
}

Data nodelist::pushnext(){
	Data null;
	null.ex = 0;
	null.ey = 0;
	null.nlist = -1;
	Node *p1;
	if (head == NULL)
		return null;
	if (current != NULL){
		p1 = current;
		current = current->next;
		return p1->data;

	}
	return null;
}

int nodelist::delall(){
	return 0;

}
nodelist::~nodelist()
{
}
