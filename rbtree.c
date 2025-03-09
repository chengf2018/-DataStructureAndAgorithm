
#include <stdio.h>
#include <stdlib.h>

/*
红黑树的性质：
1.节点是红色或者黑色
2.根节点是黑色的
3.从任一结点到其每个叶子的所有路径都包含相同数目的黑色结点
4.每个红色结点的两个子结点都是黑色
5.所有叶子节点（NIL节点）都是黑色


插入双红节点修复情况分类：
1. 叔叔节点 U 为红色
情况描述：当叔叔节点 U 是红色时，只需要进行颜色调整即可。
调整步骤：
将父节点 P 和叔叔节点 U 染成黑色。
将祖父节点 G 染成红色。
把祖父节点 G 当作新的当前节点，继续向上检查是否还存在双红冲突。

2. 叔叔节点 U 为黑色，且 N、P、G 呈直线型
情况细分：
左左情况（P 是 G 的左子节点，N 是 P 的左子节点）：
调整步骤：以祖父节点 G 为支点进行右旋操作，然后交换父节点 P 和祖父节点 G 的颜色。
右右情况（P 是 G 的右子节点，N 是 P 的右子节点）：
调整步骤：以祖父节点 G 为支点进行左旋操作，然后交换父节点 P 和祖父节点 G 的颜色。

3. 叔叔节点 U 为黑色，且 N、P、G 呈折线型
情况细分：
左右情况（P 是 G 的左子节点，N 是 P 的右子节点）：
调整步骤：先以父节点 P 为支点进行左旋操作，将其转换为左左情况，然后按照左左情况进行处理。

右左情况（P 是 G 的右子节点，N 是 P 的左子节点）：
调整步骤：先以父节点 P 为支点进行右旋操作，将其转换为右右情况，然后按照右右情况进行处理。



*/


enum RB_TREE_COLOR {
    RED = 0,
    BLACK = 1
};

typedef struct rb_node {
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
    int color;

    int key;
} rb_node_t;

typedef struct rb_tree {
    rb_node_t *root;

    int count;
} rb_tree_t;

rb_tree_t *
create_rbtree() {
    rb_tree_t *rb = malloc(sizeof(rb_tree_t));
    rb->root = NULL;
    rb->count = 0;
    return rb;
}

static void
left_rotate(rb_tree_t *rb, rb_node_t *cur_node) {
    rb_node_t *parent = cur_node->parent;
    rb_node_t *right = cur_node->right;

    cur_node->right = right->left;
    if (right->left)
        right->left->parent = cur_node;

    right->parent = parent;
    
    if (parent == NULL)
        rb->root = right;
    else if (parent->left == cur_node)
        parent->left = right;
    else
        parent->right = right;

    cur_node->parent = right;
    right->left = cur_node;
}

static void
right_rotate(rb_tree_t *rb, rb_node_t *cur_node) {
    rb_node_t *parent = cur_node->parent;
    rb_node_t *left = cur_node->left;

    cur_node->left = left->right;
    if (left->right)
        left->right->parent = cur_node;
    
    left->parent = parent;
    if (parent == NULL) rb->root = left;
    else if (parent->left == cur_node)
        parent->left = left;
    else
        parent->right = left;

    cur_node->parent = left;
    left->right = cur_node; 
}

static void
double_red_adjust(rb_tree_t *rb, rb_node_t *cur_node) {
    rb_node_t *parent = cur_node->parent;

    // 如果父亲节点是黑色，那么无需调整
    if (parent->color == BLACK) return;

    rb_node_t *grandparent = parent->parent;
    rb_node_t *uncel = (parent == grandparent->left) ? grandparent->right : grandparent->left;

    // 父亲节点是红色，那么就需要调整了
    if (uncel && uncel->color == RED) {
        //场景1：叔叔节点为红色
        parent->color = BLACK;
        uncel->color = BLACK;
        grandparent->color = RED;
        double_red_adjust(rb, grandparent);
    } else {
        if (grandparent->left == parent) {
            if (cur_node == parent->left) {
                //场景2-情况1： 叔叔节点为黑色，且当前节点是父节点的左孩子
                parent->color = BLACK;
                grandparent->color = RED;
                right_rotate(rb, grandparent);
                
            } else {
               //场景2-情况2： 叔叔节点为黑色，且当前节点是父节点的右孩子
               cur_node->color = BLACK;
               grandparent->color = RED;
               left_rotate(rb, parent);
               right_rotate(rb, grandparent);
            }
        } else {
            if (cur_node == parent->left) {
                //场景3-情况1： 叔叔节点为黑色，且当前节点是父节点的左孩子
                parent->color = BLACK;
                grandparent->color = RED;
                left_rotate(rb, grandparent);
                
            } else {
               //场景3-情况2： 叔叔节点为黑色，且当前节点是父节点的右孩子
               cur_node->color = BLACK;
               grandparent->color = RED;
               left_rotate(rb, parent);
               right_rotate(rb, grandparent);
            }
        }
        
    }
}

void
rbtree_insert(rb_tree_t *rb, int key) {
    rb_node_t *new_node = malloc(sizeof(rb_node_t));
    new_node->left = new_node->right = new_node->parent = NULL;
    new_node->color = RED;
    new_node->key = key;

    if (rb->root == NULL) {
        rb->root = new_node;
        new_node->color = BLACK;
        return;
    }

    rb_node_t *p = rb->root;
    rb_node_t *parent = NULL;

    // 找到插入位置
    while (p) {
        parent = p;
        p = (key < p->key) ? p->left : p->right;
    }

    new_node->parent = parent;
    if (key < parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;

    double_red_adjust(rb, new_node);

    if (rb->root->color == RED)
        rb->root->color = BLACK;
}

void
rbtree_search() {

}

void
rbtree_delete() {

}


int
main(int argc, char ** argv) {

    rb_tree_t *rb = create_rbtree();

    int i;
    for (i=0; i<10; i++) {
         rbtree_insert(rb, i);
    }

    printf("run over\n");
    return 0;
}
