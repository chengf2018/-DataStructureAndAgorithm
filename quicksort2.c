#include <stdio.h>
void
swap(int array[], int a, int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void
quick_sort(int array[], int left, int right) {
    if (left >= right) return;
    int i, j=left+1;
    int key = array[left];
    for (i=left+1; i<right; i++) {
        if (array[i] < key) {
            swap(array, i, j++);
        }
    }
    swap(array, j-1, left);
    quick_sort(array, left, j-1);
    quick_sort(array, j, right);
}

int
main(int argc, char **argv) {
    int array[] = {32,10, 8, 3, 2, 1, 6, 0};
    quick_sort(array, 0, sizeof(array)/sizeof(int));

    for (int i = 0; i < sizeof(array)/sizeof(int); i++) {
        printf("%d   ", array[i]);
    }
    printf("\n");
    return 0;
}
