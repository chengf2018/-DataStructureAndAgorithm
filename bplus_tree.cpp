#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

using namespace std;

#define K 3   //K degree b+tree

typedef struct _bnode {
	struct _bnode *father;
	struct _bnode *next;
	struct _bnode *last;
	int key[K+1];
	int key_count;
	struct _bnode *p[K+1];
	int btype;//1.internal node 2.leaf node
} bnode;

typedef struct _btree {
	struct _bnode *root;
} btree;

bnode *new_node(int type) {
	bnode *bn = new bnode();
	bn->father = nullptr;
	bn->next = nullptr;
	bn->last = nullptr;
	memset(bn->key, 0, sizeof(bn->key)*sizeof(int));
	bn->key_count = 0;
	for (int i=0; i<K+1; i++) {
		bn->p[i] = nullptr;
	}
	bn->btype = type;
	return bn;
}

void free_node(bnode *node) {
	if (!node) return;
	//don't need to free (K+1)th node
	for (int i=0; i<K; i++) {
		if (!node->p[i]) break;
		free_node(node->p[i]);
	}
	delete node;
}

void init_btree(btree* bt) {
	bt->root = new_node(2);
}

void free_btree(btree* bt) {
	if (!bt) return;
	free_node(bt->root);
}

static insert_index_to_internal(btree* bt, bnode *bn, int key) {
}

static void insert_to_internal(btree* bt, bnode* bn, int key) {
	assert(bn->btype == 1);
}

static void insert_to_leaf(btree* bt, bnode* bn, int key) {
	assert(bn->btype == 2);

	//find the insert pos
	int pos = 0;
	for (pos=0; pos<bn->key_count; pos++) {
		if (key > bn->key[pos]) break;
	}

	if (pos < bn->key_count) {
		for (int i=bn->key_count; i>pos; i--) {
			bn->key[i] = bn->key[i-1];
		}
	}
	bn->key[pos] = key;
	bn->key_count ++;

	//Is full?
	if (bn->key_count == K) {
		//split node
		bnode* nn= new_node(2);
		int j=0;
		for (int i=K/2; i<bn->key_count; i++) {
			nn->key[j++] = bn->key[i];
			bn->key[i] = 0;
		}
		bn->key_count -= std::ceil(K/2);
		nn->key_count = j;

		//when no father:
		//the floor(K/2) node on left remain at original node
		//move the ceil(K/2) node of right to new node
		if (!bn->father) {
			bnode *father = new_node(1);
			father->key[0] = nn->key[0];
			father->key_count = 1;
			father->p[0] = bn;
			father->p[1] = nn;
			bn->father = father;
			nn->father = father;
			bt->root = father;
		} else {
			insert_index_to_internal(bt, bn->father, nn);
		}
	}
}


void btree_insert(btree* bt, int key) {
	bnode *p = bt->root;
	if (p->btype == 1) insert_to_internal(bt, p, key);
	else               insert_to_leaf(bt, p, key);
}

int btree_remove(btree* bt, int key) {
	return 0;
}

int btree_find(btree* bt, int left, int right) {
	return 0;
}

int main(int argc, char** argv) {
	btree bt;
	init_btree(&bt);
	
	free_btree(&bt);
	std::cout << "test";
	return 0;
}
