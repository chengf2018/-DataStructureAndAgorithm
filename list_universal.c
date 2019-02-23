#include "list_universal.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



void *
myMalloc(unsigned int size) {
	void * mem = malloc(size);
	assert(mem);
	return mem;
}

void
myFree(void * p) {
	free(p);
}

void *
dataCopy(void * data, unsigned int size) {
	void * datacp = myMalloc(size);
	memcpy(datacp, data, size);
	return datacp;
}

NODE *
findNode(LIST * list, NODE *node) {
	NODE *p = list->head;
	if (!list || !node) return 0;
	while (p) {
		if (p == node) {
			return p;
		}
		p = p->next;
	}
	return 0;
}

NODE *
NewNode() {
	void *node = myMalloc(sizeof(NODE));
	memset(node, 0, sizeof(NODE));
	return (NODE *)node;
}

void
FreeNode(NODE * node) {
	if (node) {
		if (node->data) myFree(node->data);
		myFree(node);
	}
}

LIST *
NewList() {
	void * list = myMalloc(sizeof(LIST));
	memset(list, 0, sizeof(LIST));
	return (LIST *)list;
}

void
FreeList(LIST * list) {
	if (list) myFree(list);
}



NODE *
List_PushHead(LIST * list, void * data, unsigned int size) {
	NODE *newnode;
	if (!list || !data) return 0;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	newnode = NewNode();
	newnode->data = dataCopy(data, size);;
	newnode->datasize = size;
	newnode->next = list->head;

	if (list->head) {
		list->head->last = newnode;
		list->head = newnode;
	} else {
		list->head = list->tail = newnode;
	}

	list->size ++;
	return newnode;
}

NODE *
List_PushTail(LIST * list, void * data, unsigned int size) {
	NODE * newnode;
	if (!list || !data) return 0;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	newnode = NewNode();
	newnode->data = dataCopy(data, size);
	newnode->datasize = size;
	newnode->last = list->tail;

	if (list->tail) {
		list->tail->next = newnode;
		list->tail = newnode;
	} else {
		list->tail = list->head = newnode;
	}
	list->size ++;
	return newnode;
}

NODE *
List_PopHead(LIST * list) {
	NODE *popnode;
	if (!list || !list->head) return 0;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	popnode = list->head;
	list->head = popnode->next;

	if (list->head) {
		list->head->last = 0;
	} else {
		list->tail = 0;
	}
	list->size --;
	return popnode;
}

NODE *
List_PopTail(LIST * list) {
	NODE *popnode;
	if (!list || !list->tail) return 0;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	popnode = list->tail;
	list->tail = popnode->last;

	if (list->tail) {
		list->tail->next = 0;
	} else {
		list->head = 0;
	}
	list->size --;
	return popnode;
}

void
List_Remove(LIST * list, NODE * node) {
	if (!list || !node) return;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	
	if (!findNode(list, node)) return;

	if (node->last) {
		node->last->next = node->next;
	}
	if (node->next) {
		node->next->last = node->last;
	}

	if (node == list->head) {
		list->head = node->next;
	}
	if (node == list->tail) {
		list->tail = node->last;
	}
	FreeNode(node);
	list->size --;
}

NODE *
List_Find(LIST * list, void * data, unsigned int size) {
	NODE *p = list->head;
	if (!list || !data) return 0;
	
	while (p) {
		if (p->datasize == size && memcmp(p->data, data, size) == 0) {
			return p;
		}
		p = p->next;
	}
	return 0;
}

NODE *
List_Insert(LIST * list, NODE * node, void * data, unsigned int size) {
	NODE * newnode;
	if (!list || !node || !data) return 0;
	assert(!(list->head != list->tail && (!list->head || !list->tail)));
	newnode = NewNode();
	newnode->data = dataCopy(data, size);
	newnode->datasize = size;
	newnode->next = node;
	newnode->last = node->last;
	if (node->last) {
		node->last->next = newnode;
	}
	node->last = newnode;

	if (node == list->head) {
		list->head = newnode;
	}
	
	list->size ++;
	return newnode;
}

unsigned int
List_Size(LIST * list) {
	if (!list) return 0;
	return list->size;
}