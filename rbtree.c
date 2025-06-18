
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    assert(cur_node && cur_node->right);
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
    
    assert(rb->root->parent == NULL);
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

    assert(rb->root->parent == NULL);
}

static void
fix_double_red(rb_tree_t *rb, rb_node_t *cur_node) {
    rb_node_t *parent = cur_node->parent;

    // 如果父亲节点是黑色，那么无需调整
    if (!parent || parent->color == BLACK) return;

    rb_node_t *grandparent = parent->parent;
    rb_node_t *uncel = (parent == grandparent->left) ? grandparent->right : grandparent->left;

    // 父亲节点是红色，那么就需要调整了
    if (uncel && uncel->color == RED) {
        //场景1：叔叔节点为红色
        parent->color = BLACK;
        uncel->color = BLACK;
        grandparent->color = RED;
        fix_double_red(rb, grandparent);
    } else {
        if (grandparent->left == parent) {
            if (cur_node == parent->left) {
                //场景2-左左
                parent->color = BLACK;
                grandparent->color = RED;
                right_rotate(rb, grandparent);
                
            } else {
               //场景3-左右
               left_rotate(rb, parent);
               fix_double_red(rb, parent);
            }
        } else {
            if (cur_node == parent->right) {
                //场景2-右右
                parent->color = BLACK;
                grandparent->color = RED;
                left_rotate(rb, grandparent);
            } else {
                //场景3-右左
                right_rotate(rb, parent);
                fix_double_red(rb, parent);
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
        rb->count++;
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

    fix_double_red(rb, new_node);

    if (rb->root->color == RED)
        rb->root->color = BLACK;
    rb->count++;
}

rb_node_t *
rbtree_search(rb_tree_t *rb, int key) {
    rb_node_t *p = rb->root;
    while (p) {
        if (key == p->key) {
            printf("Found key: %d\n", p->key);
            return p;
        } else if (key < p->key) {
            p = p->left;
        } else {
            p = p->right;
        }
    }
    printf("Key %d not found in the tree.\n", key);
    return NULL;
}

static rb_node_t *
find_min_node(rb_node_t *node) {
    rb_node_t *successor = node->right;
    if (successor) {
        while (successor->left) {
            successor = successor->left;
        }
        return successor;
    }
    return NULL;
}

static void
do_delete_node(rb_tree_t *rb, rb_node_t *node) {
    assert(rb && node);
    if (node->parent) {
        if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
    } else {
        rb->root = NULL; // 删除根节点
    }       
    free(node);
}

static void
shift_up_remove_node(rb_tree_t *rb, rb_node_t *node) {
    assert(rb && node);
    if (node->color == RED || node == rb->root) {
        node->color = BLACK; // 将当前节点染成黑色
        return;
    }

    rb_node_t *parent = node->parent;
    rb_node_t *sibling = (parent->left == node) ? parent->right : parent->left;
    assert(parent);
    assert(sibling);

    // 至少有一个红孩子的情况
    if (sibling == parent->left && sibling->color == BLACK) {
        if (sibling->left && sibling->left->color == RED) {
            //LL型
            sibling->left->color = BLACK;
            sibling->color = parent->color;
            parent->color = BLACK;
            right_rotate(rb, parent);
            return;
        } else if (sibling->right && sibling->right->color == RED) {
            //LR型
            sibling->right->color = parent->color;
            parent->color = BLACK;
            left_rotate(rb, sibling);
            right_rotate(rb, parent);
            return;
        }
    } else if (sibling == parent->right && sibling->color == BLACK) {
        if (sibling->right && sibling->right->color == RED) {
            //RR型
            sibling->right->color = BLACK;
            sibling->color = parent->color;
            parent->color = BLACK;
            left_rotate(rb, parent);
            return;
        } else if (sibling->left && sibling->left->color == RED) {
            //RL型
            sibling->left->color = parent->color;
            parent->color = BLACK;
            right_rotate(rb, sibling);
            left_rotate(rb, parent);
            return;
        }
    }

    //兄弟节点都是黑孩子的情况
    if (sibling->color == BLACK &&
        (sibling->left == NULL || sibling->left->color == BLACK) &&
        (sibling->right == NULL || sibling->right->color == BLACK)) {
        sibling->color = RED;
        shift_up_remove_node(rb, parent);
        return;
    }

    if (sibling->color == RED) {
        sibling->color = BLACK;
        parent->color = (parent->color == RED) ? BLACK : RED;
        if (parent->left == node) {
            left_rotate(rb, parent);
        } else {
            right_rotate(rb, parent);
        }
        shift_up_remove_node(rb, node);
        return;
    }

    //不应该跑到这里
    printf("what the fuck?\n");
    assert(node);
}

static void
do_delete(rb_tree_t *rb, rb_node_t *node) {
    rb_node_t *child = NULL;

    // 找到后继节点
    rb_node_t *min_node = find_min_node(node);
    if (min_node != NULL) {
        node->key = min_node->key;
        node = min_node; // 删除后继节点
    }

    //情况0：被删除节点是根节点
    if (node == rb->root) {
        do_delete_node(rb, node);
        assert(rb->root == NULL);
        return;
    }

    //情况1：被删除的节点是红色节点:直接删除即可
    //如果被删除的节点是红色节点，那么一定是没有孩子的，不然这颗红黑树不符合黑路同性质,和不会有两个连续红色节点的性质
    if (node->color == RED) {
        assert(node->left == NULL && node->right == NULL);
        do_delete_node(rb, node);
        return;
    }
    
    //其他情况删除节点一定是黑色节点
    assert(node->color == BLACK);

    //情况2：被删除的节点是黑色节点，且只有一个子节点：直接用子节点替代，并将代替节点变黑
    if (node->left == NULL || node->right == NULL) {
        child = (node->left) ? node->left : node->right;
        if (child) { // 拥有至少一个子节点
            child->parent = node->parent; // 将子节点的父节点指向被删除节点的父节点
            child->color = BLACK; // 将子节点染成黑色
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = child;
                } else {
                    node->parent->right = child;
                }
            } else {
                rb->root = child; // 删除根节点
            }
            free(node);
            return;
        }
    }

    //这种情况下的黑色节点必定是没有孩子的，仔细想想为什么？
    assert(node->left == NULL && node->right == NULL);
    rb_node_t *parent = node->parent;
    rb_node_t *sibling = (parent->left == node) ? parent->right : parent->left;

    //至少有一个红色或者黑色的兄弟节点，如果没有兄弟节点那就不满足黑路同性质了
    assert(sibling);
    assert(parent);
    
    //情况3：被删除的节点是黑色节点，兄弟节点是黑色，且兄弟节点至少一个子节点是红色:
    //LL型：将兄弟节点染成父节点的颜色，兄弟节点的红色子节点染成黑色，父节点染成黑色；
    //RR型：将兄弟节点染成父节点的颜色，兄弟节点的红色子节点染成黑色，父节点染成黑色；
    //LR型：将兄弟节点的右孩子染成父节点的颜色，父节点染成黑色，左旋兄弟节点，然后右旋父节点；
    //RL型：将兄弟节点的左孩子染成父节点的颜色，父节点染成黑色，右旋兄弟节点，然后左旋父节点；
    if (parent->left == sibling && sibling->color == BLACK) {
        if (sibling->left && sibling->left->color == RED) {//LL型
            sibling->left->color = BLACK;
            sibling->color = parent->color;
            parent->color = BLACK;
            right_rotate(rb, parent);
            //删除node节点
            do_delete_node(rb, node);
            return;
        } else if (sibling->right && sibling->right->color == RED) {//LR型
            sibling->right->color = parent->color;
            parent->color = BLACK;
            left_rotate(rb, sibling);
            right_rotate(rb, parent);
            //删除node节点
            do_delete_node(rb, node);
            return;
        }
    } else if (parent->right == sibling && sibling->color == BLACK) {
        if (sibling->right && sibling->right->color == RED) {//RR型
            sibling->right->color = BLACK;
            sibling->color = parent->color;
            parent->color = BLACK;
            left_rotate(rb, parent);
            //删除node节点
            do_delete_node(rb, node);
            return;

        } else if (sibling->left && sibling->left->color == RED) {//RL型
            sibling->left->color = parent->color;
            parent->color = BLACK;
            right_rotate(rb, sibling);
            left_rotate(rb, parent);
            //删除node节点
            do_delete_node(rb, node);
            return;
        }
    }

    //情况4：被删除的节点是黑色节点，兄弟节点是黑色，且兄弟节点的两个子节点都是黑色:将兄弟节点染成红色，往上调整
    if (sibling->color == BLACK &&
        (!sibling->left || sibling->left->color == BLACK) &&
        (!sibling->right || sibling->right->color == BLACK)) {
        
        sibling->color = RED; // 将兄弟节点染成红色
        do_delete_node(rb, node);
        shift_up_remove_node(rb, parent);
        return;
    }
    
    //情况5：被删除的节点是黑色节点，兄弟节点是红色:将兄父变色，父节点朝删除节点方向旋转，然后继续对当前节点调整
    //删除节点在父节点的右边：父节点右旋
    //删除节点在父节点的左边：父节点左旋
    if (sibling->color == RED) {
        sibling->color = BLACK;
        parent->color = (parent->color == RED) ? BLACK : RED;
        if (parent->left == node) {
            left_rotate(rb, parent);
        } else {
            right_rotate(rb, parent);
        }
        shift_up_remove_node(rb, node);
        do_delete_node(rb, node);
        return;
    }

    //不应该跑到这里
    printf("what the fuck!?\n");
    assert(node);
}

int
rbtree_delete(rb_tree_t *rb, int key) {
    rb_node_t *p = rb->root;
    while (p) {
        if (key == p->key) {
            do_delete(rb, p);
            rb->count--;
            printf("Delete key: %d\n", p->key);
            return 0;
        } else if (key < p->key) {
            p = p->left;
        } else {
            p = p->right;
        }
    }
    printf("Delete key: %d not found\n", key);
    return -1;
}


int
main(int argc, char ** argv) {

    rb_tree_t *rb = create_rbtree();

    int i;
    for (i=0; i<10; i++) {
         rbtree_insert(rb, i);
    }

    rbtree_delete(rb, 4);
    rbtree_delete(rb, 5);
    rbtree_delete(rb, 6);
    rbtree_delete(rb, 99);

    printf("test done\n");
    return 0;
}
