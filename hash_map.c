#include "hash_map.h"

void * 
myMalloc(unsigned int size) {
	return malloc(size);
}

void 
myFree(void *mem) {
	if (mem) free(mem);
}

HASH_NODE * newNodeList(unsigned int size) {
	HASH_NODE * node = myMalloc(sizeof(HASH_NODE)*size);
	memset(node, 0, sizeof(HASH_NODE)*size);
	return node;
}

void freeNodeList(HASH_NODE * list, unsigned int size) {
	int i;
	HASH_NODE *p;
	if (!list) return;
	for (i = 0; i < size; i++) {
		p = &list[i];

		while (p) {
			if (p->key) myFree(p->key);
			if (p->value) myFree(p->value);
			p = p->next;
		}
	}

	myFree(list);
}

HASH_MAP * 
newMap(unsigned int size, CMP_FUNC compare, HASH_FUNC hashfunc) {
	HASH_MAP * map = (HASH_MAP *)myMalloc(sizeof(HASH_MAP));
	map->list = newNodeList(size);
	map->size = size;
	map->equal = compare;
	map->hashfunc = hashfunc;
	return map;
}

void
freeMap(HASH_MAP * map) {
	myFree(map);
}

HASH_NODE *
newNode() {
	HASH_NODE * node = (HASH_NODE *)myMalloc(sizeof(HASH_NODE));
	memset(node, 0, sizeof(HASH_NODE));
	return node;
}

void
freeNode(HASH_NODE *node) {
	if (!node) return;
	if (node->key) myFree(node->key);
	if (node->value) myFree(node->value);
	myFree(node);
}

HASH_MAP * 
HashMap_Create(unsigned size, CMP_FUNC compare, HASH_FUNC hashfunc) {
	return newMap(size, compare, hashfunc);
}

void
HashMap_Destroy(HASH_MAP * map) {
	if (!map) return;
	if (map->list) freeNodeList(map->list, map->size);
	freeMap(map);
}

unsigned int 
HashMap_GetHashValue(HASH_MAP * map, void * key) {
	if (!map || !key) return 0;
	return map->hashfunc(key) % map->size;
}

HASH_NODE *
keyIsExist(HASH_MAP * map, HASH_NODE * list, void * key) {
	HASH_NODE *p;
	if (!map || !list || !key) return 0;
	p = list;
	while (p) {
		if (p->key && map->equal(key, p->key)) {
			return p;
		}

		p = p->next;
	}
	return 0;
}

void
addKeyToList(HASH_MAP * map, HASH_NODE * list, void * key, void * value) {
	HASH_NODE * newnode;
	if (!map || !list || !key || !value) return;
	newnode = newNode();
	newnode->next = list->next;
	newnode->key = list->key;
	newnode->value = list->value;

	list->next = newnode;
	list->key = key;
	list->value = value;

}

void 
HashMap_Set(HASH_MAP * map, void * key, void * value) {
	HASH_NODE *p;

	if (!map || !key) return;
	unsigned int hashValue = HashMap_GetHashValue(map, key);
	p = keyIsExist(map, &map->list[hashValue], key);

	if (p) {
		if (p->key) myFree(p->key);
		if (p->value) myFree(p->value);
		p->key = key;
		p->value = value;
	} else {
		addKeyToList(map, &map->list[hashValue], key, value);
	}
}

void *
HashMap_Get(HASH_MAP * map, void * key) {
	if (!map || !key) return 0;
	HASH_NODE *p;
	unsigned int hashValue = HashMap_GetHashValue(map, key);
	p = keyIsExist(map, &map->list[hashValue], key);
	if (p) {
		return p->value;
	}
	return 0;
}

void
HashMap_Erase(HASH_MAP * map, void * key) {
	HASH_NODE *p, *last, *temp;

	if (!map || !key) return;
	unsigned int hashValue = HashMap_GetHashValue(map, key);
	p = &map->list[hashValue];
	last = 0;
	while (p) {
		if (p->key && map->equal(key, p->key)) {
			if (last) {
				last->next = p->next;
				freeNode(p);
			} else {
				if (p->key) myFree(p->key);
				
				if (p->value) myFree(p->value);
				
				if (p->next) {
					p->key = p->next->key;
					p->value = p->next->value;
					temp = p->next;
					p->next = p->next->next;
					myFree(temp);
				} else {
					p->key = 0;
					p->value = 0;
				}
			}
			
			return;
		}
		last = p;
		p = p->next;
	}
}

void *
NewKey(void * key, unsigned int size) {
	if (!key) return 0;
	void *newk = myMalloc(size);
	memcpy(newk, key, size);
	return newk;
}

void *
NewValue(void * value, unsigned int size) {
	if (!value) return 0;
	void *newv = myMalloc(size);
	memcpy(newv, value, size);
	return newv;
}