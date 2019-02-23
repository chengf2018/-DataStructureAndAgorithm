#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//这段程序存在以下问题,因为只是用于学习，所以以下问题都可以忽略
//1.函数递归容易出现的问题就是栈溢出，所以实际应用最好不要用函数递归
//可以改用栈+循环的方式，例如：
//stack<DATATYPE> s;
//s.push(...)
//while(!s.empty()) {
//	DATATYPE param = s.pop();
//	//deal...
//	if (<...>) {
//		s.push(...)
//	} else {
//		...
//	}
//}
//2.还有一个问题就是内存碎片，长期运行并且经常操作内存的程序，应当采用
//内存池设计，减少内存碎片

typedef int KEYTYPE;
typedef int DATATYPE;
typedef int BOOL;
typedef struct _tree_node {
	KEYTYPE key;
	DATATYPE data;
	struct _tree_node *left;
	struct _tree_node *right;
	struct _tree_node *parent;
}TREENODE;


typedef struct _binary_tree {
	TREENODE *head;
}BINARYTREE;

//Intrinsic function
static size_t _mem_count = 0;
void *myMalloc(size_t size) {
	_mem_count++;
	return malloc(size);
}

void myFree(void *p) {
	_mem_count--;
	free(p);
}

//free node
void _free_node(TREENODE *node) {
	if (!node) return;

	_free_node(node->left);
	_free_node(node->right);

	myFree(node);
}

//total node count
int _total_node(TREENODE *node) {
	if (!node) return 0;
	return _total_node(node->left) + _total_node(node->right) + 1;
}

//calc node depth
int _depth_node(TREENODE *node) {
	if (!node) return 0;

	int depth1 = _depth_node(node->left);
	int depth2 = _depth_node(node->right);
	return (depth1 > depth2) ? (depth1 + 1) : (depth2 + 1);
}

//External function

//Create
BINARYTREE *BinaryTree_Create() {
	BINARYTREE *newtree = (BINARYTREE *)myMalloc(sizeof(BINARYTREE));
	newtree->head = 0;
	return newtree;
}

//Delete
void BinaryTree_Delete(BINARYTREE *tree) {
	if (!tree) return;

	_free_node(tree->head);

	myFree(tree);
}

//Is empty
BOOL BinaryTree_Empty(BINARYTREE *Tree) {
	if (!Tree) return true;
	return Tree->head == 0;
}

//Set key : data
int BinaryTree_Set(BINARYTREE *tree, KEYTYPE key, DATATYPE data) { 
	if (!tree) return -1;

	TREENODE *temp = tree->head;
	TREENODE *parent = 0;
	while(temp) {
		parent = temp;
		if (key < temp->key) {
			temp = temp->left;
		} else if (key > temp->key) {
			temp = temp->right;
		} else {
			temp->data = data;
			return -1;
		}
	}

	TREENODE *newnode = (TREENODE*)myMalloc(sizeof(TREENODE));
	newnode->key = key;
	newnode->data = data;
	newnode->parent = parent;
	newnode->left = 0;
	newnode->right = 0;

	if (parent == 0) {
		tree->head = newnode;
		return 0;
	}

	if (key < parent->key) {
		parent->left = newnode;
	} else {
		parent->right = newnode;
	}
	return 0;
}

//Remove node by key
int BinaryTree_Remove(BINARYTREE *tree, KEYTYPE key) {
	if (!tree) return -1;

	TREENODE *temp = tree->head;
	while(temp) {
		if (key < temp->key) {
			temp = temp->left;
		} else if (key > temp->key) {
			temp = temp->right;
		} else {
			//finded
			if (temp->left && temp->right) {
				TREENODE *j = temp->right;
				while (!j->left) {
					j = j->left;
				}

				if (temp->parent) {
					if (temp == temp->parent->left) {
						temp->parent->left = j;
					} else {
						temp->parent->right = j;
					}
				} else {
					tree->head = j;
				}

				if (j == temp->right) {
					j->left = temp->left;
					temp->left->parent = j;
				} else {
					j->parent->left = j->right;
					j->left = temp->left;
					temp->left->parent = j;
					j->right = temp->right;
				}

			} else if (temp->left) {
				if (temp->parent) {
					if (temp == temp->parent->left) {
						temp->parent->left = temp->left;
					} else {
						temp->parent->right = temp->left;
					}
				} else {
					tree->head = temp->left;
				}
				temp->left->parent = temp->parent;
			} else if (temp->right) {
				if (temp->parent) {
					if (temp == temp->parent->left) {
						temp->parent->left = temp->right;
					} else {
						temp->parent->right = temp->right;
					}
				} else {
					tree->head = temp->right;
				}
				temp->right->parent = temp->parent;
			} else {
				if (temp->parent) {
					if (temp == temp->parent->left) {
						temp->parent->left = 0;
					} else {
						temp->parent->right = 0;
					}
				}
				else {
					tree->head = 0;
				}
			}

			myFree(temp);
			return 0;
		}

	}

	return -1;
}

//Find node by key
TREENODE *BinaryTree_Find(BINARYTREE *tree, KEYTYPE key) {
	if (!tree) return 0;

	TREENODE *temp = tree->head;

	while(temp) {
		if (key < temp->key) {
			temp = temp->left;
		} else if (key > temp->key) {
			temp = temp->right;
		} else {
			//finded
			return temp;
		}
	}
	return 0;
}

//Total count
int BinaryTree_Count(BINARYTREE *tree) {
	return _total_node(tree->head);
}

//Get tree depth
int BinaryTree_Depth(BINARYTREE *tree) {
	if (!tree) return 0;
	return _depth_node(tree->head);
}

//Test function

void _out_node(TREENODE *node) {
	if (!node) return;
	
	printf("%d\t", node->key);
	_out_node(node->left);

	_out_node(node->right);
	
}

void _test_dump(BINARYTREE *tree) {
	if (!tree) return;

	_out_node(tree->head);
	printf("\n");
}

int main(int argc, char **argv) {
	BINARYTREE *tree;
	tree = BinaryTree_Create();

	//test code

	// BinaryTree_Set(tree, 1, 999);
	
	for (int i = 0; i < 50000; i++) {
		BinaryTree_Set(tree, i, i);
	}

	for (int i = 0; i < 50000; i++)
		BinaryTree_Remove(tree, i);

	// for (int i = 0; i < 50000; i++) {
	// 	TREENODE *node = BinaryTree_Find(tree, i);
	// 	if (node != 0) {
	// 		//printf("%d\t", node->data);
	// 		BinaryTree_Remove(tree, node->key);
	// 	}
	// }

	// int count = BinaryTree_Count(tree);
	// printf("count:%d\n", count);

	// int depth = BinaryTree_Depth(tree);
	// printf("depth:%d\n", depth);

	// BinaryTree_Remove(tree, 1);
	// BinaryTree_Remove(tree, 9);
	// BinaryTree_Remove(tree, 3);
	// BinaryTree_Remove(tree, 2);
	// BinaryTree_Remove(tree, 4);
	
	
	//_test_dump(tree);
	BinaryTree_Delete(tree);

	printf("\nusing mem count:%d\n", _mem_count);

	assert(_mem_count == 0);
	getchar();
	return 0;
}