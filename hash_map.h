#ifndef _HASH_MAP_H
#define _HASH_MAP_H

#include <stdlib.h>
#include <string.h>

typedef struct hash_node {
	void * key;
	void * value;
	struct hash_node *next;
}HASH_NODE;

typedef int(*CMP_FUNC)(void *,void *);
typedef unsigned(*HASH_FUNC)(void *);

typedef struct hash_map {
	HASH_NODE * list;
	unsigned int size;
	CMP_FUNC equal;
	HASH_FUNC hashfunc;
}HASH_MAP;

void *NewKey(void * key, unsigned int size);
void *NewValue(void * value, unsigned int size);

HASH_MAP * HashMap_Create(unsigned size, CMP_FUNC compare, HASH_FUNC hashfunc);
void HashMap_Destroy(HASH_MAP * map);

unsigned int HashMap_GetHashValue(HASH_MAP * map, void * key);

void HashMap_Set(HASH_MAP * map, void * key, void * value);
void * HashMap_Get(HASH_MAP * map, void * key);
void HashMap_Erase(HASH_MAP * map, void * key);

#endif //_HASH_MAP_H