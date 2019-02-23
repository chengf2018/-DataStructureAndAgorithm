//冒泡排序的实现
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int DATATYPE;
typedef int(*CMP_FUNC)(DATATYPE, DATATYPE);

static CMP_FUNC cmp_func;

int _default_compare(DATATYPE n1, DATATYPE n2) {
	return n1 < n2;
}

//Bubble Sort
void BubbleSort(DATATYPE *array, int left, int right, CMP_FUNC compare) {
	if (!array || left >= right || left <= 0) return;

	cmp_func = compare ? compare : _default_compare;
	
	for (int i = left - 1; i < right; i++) {
		int isFinish = 1;
		for (int j = left - 1; j < right - i - 1; j++) {
			if (cmp_func(array[j+1], array[j])) {
				DATATYPE temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
				isFinish = 0;
			}
		}

		//如果没有交换数据，说明已经排好了
		if (isFinish) {
			break;
		}
	}
}

// test code

void dump_array(DATATYPE *array, int count) {
	for (int i = 0; i < count; i++) {
		printf("%d\t", array[i]);
	}
}

int main(int argc, char **argv) {
	DATATYPE array[] = {2, 1, 3, 4, 5, 6, 7, 244};
	int count = sizeof(array) / sizeof(array[0]);

	BubbleSort(array, 1, count, 0);

	dump_array(array, count);

	system("pause");
	return 0;
}