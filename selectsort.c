//选择排序实现
#include <stdio.h>
#include <stdlib.h>

typedef int DATATYPE;

typedef int (*CMP_FUNC)(DATATYPE, DATATYPE);

static CMP_FUNC cmp_func;

//Default compare function
int DefaultCompare(DATATYPE n1, DATATYPE n2) {
	return n1 < n2;
}

//Select Sort
void SelectSort(DATATYPE *array, int left, int right, CMP_FUNC compare) {
	if (!array || left >= right || left <= 0) return;

	cmp_func = compare ? compare : DefaultCompare;

	for (int i = left - 1; i < right; i++) {
		int pos = i;//选择排序：选出最大/最小的索引，然后和第i个进行交换
		for (int j = i + 1; j < right; j++) {
			if (cmp_func(array[j], array[pos])) {
				pos = j;
			}
		}
		DATATYPE temp = array[i];
		array[i] = array[pos];
		array[pos] = temp;
	}
}


// test code
void dump_array(DATATYPE *array, int count) {
	if(!array) return;
	for (int i = 0; i < count; i++) {
		printf("%d\t", array[i]);
	}
}

int main(int argc, char **argv) {
	DATATYPE array[] = {564, 548, 568, 124, 564, 333, 123};
	int count = sizeof(array) / sizeof(array[0]);

	SelectSort(array, 1, count, 0);

	dump_array(array, count);

	system("pause");
	return 0;
}