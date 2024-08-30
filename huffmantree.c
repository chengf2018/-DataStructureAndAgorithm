#include <stdio.h>

typedef struct {
    int weight;
    int left_child;
    int right_child;
    int parent;
    char c;
} NodeTree;


void
init(NodeTree *set, size_t count) {
    for (size_t i = 0; i < count; i++) {
        set[i].weight = -1;
        set[i].left_child = -1;
        set[i].right_child = -1;
        set[i].parent = -1;
        set[i].c = 0;
    }
}

void
init_test_data(NodeTree *set) {
    set[0].c = 'a';
    set[0].weight = 10;
    set[1].c = 'b';
    set[1].weight = 20;
    set[2].c = 'c';
    set[2].weight = 30;
    set[3].c = 'd';
    set[3].weight = 40;
    set[4].c = 'e';
    set[4].weight = 50;
}

#define NODE_NUM 5
void
dump_tree(NodeTree *set) {
    for (int i = 0; i < NODE_NUM*2; i++) {
        NodeTree *cur = &set[i];
        printf("Num.%d weight:%d, left:%d, right:%d, value:%c\n", i, cur->weight,
               cur->left_child, cur->right_child, cur->c);
    }
}


int
main(int argc, char **argv) {
    NodeTree set[NODE_NUM * 2];
    init(set, NODE_NUM * 2);
    init_test_data(set);
    
    int n = NODE_NUM;
    int w1, w2, x1, x2;
    int p=0;
    for (int i = NODE_NUM-1; i >= 0; i--) {
        w1=-1; w2=-1;
        x1=-1; x2=-1;
        int np = NODE_NUM + p;
        for (int j = 0; j < np; j++) {
            NodeTree *cur = &set[j];
            if (cur->weight != -1 && cur->parent == -1) {
                if (cur->weight < w1 || w1 == -1) {
                    w2 = w1;  x2 = x1;
                    w1 = cur->weight;
                    x1 = j;
                } else if (cur->weight < w2 || w2 == -1) {
                    w2 = cur->weight;
                    x2 = j;
                }
            }
        }
        //NODE_NUM+p is free node
        if (x1 != -1 && x2 != -1) {
            set[np].weight = set[x1].weight + set[x2].weight;
            set[np].left_child = x1;
            set[np].right_child = x2;
            set[x1].parent = np;
            set[x2].parent = np;
            p++;
        }
    }
    dump_tree(set);
    printf("p:%d\n", NODE_NUM+p-1);
    return 0;
}
