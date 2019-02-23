/*
	universal list
	100% ANSI-C, auto copy data to node and free it.
 */

#ifndef _LIST_UNIVERSAL_H
#define _LIST_UNIVERSAL_H

typedef struct _node {
	void * data;
	unsigned int datasize;
	struct _node * last;
	struct _node * next;
}NODE;

typedef struct _list {
	NODE * head;
	NODE * tail;
	unsigned int size;
}LIST;

LIST * NewList();
void FreeList(LIST * list);
NODE * NewNode();
void FreeNode(NODE * node);

NODE * List_PushHead(LIST * list, void * data, unsigned int size);
NODE * List_PushTail(LIST * list, void * data, unsigned int size);
// The pop node needs to free it
NODE * List_PopHead(LIST * list);
// The pop node needs to free it
NODE * List_PopTail(LIST * list);
void List_Remove(LIST * list, NODE * node);
NODE * List_Find(LIST * list, void * data, unsigned int size);
NODE * List_Insert(LIST * list, NODE * node, void * data, unsigned int size);
unsigned int List_Size(LIST * list);

#endif //_LIST_UNIVERSAL_H