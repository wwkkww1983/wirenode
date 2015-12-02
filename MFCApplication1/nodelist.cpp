#include "stdafx.h"
#include "nodelist.h"


NL::NL()
{
	head = NULL;
	current = NULL;
}

int NL::add(Data data){
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

int NL::del(Node *node){
	if (node == NULL)//���ҿսڵ�
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
		return -1;//δ�ҵ�
	}
	else{
		p1->next = p1->next->next;
		current = head;
		return 1;
	}
}

int NL::del(Data data){
	if (del(search(data))){
		return 1;
	};
	return 0;
}

Node* NL::search(Data data){
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

Data NL::pushnext(){
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
	current = head;
	return null;
}

int NL::delall(){
	return 0;

}

Node* NL::search(int nlist){
	Node* p1 = head;
	while (p1 != NULL&&p1->data.nlist != nlist){
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
NL::~NL()
{
}
