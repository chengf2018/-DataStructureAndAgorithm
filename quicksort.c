//快速排序算法的递归实现
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int DATATYPE;
typedef int(*CMP_FUNC)(DATATYPE, DATATYPE);

static CMP_FUNC cmp_func;

//Default compare fucntion
int _default_compare(DATATYPE n1, DATATYPE n2) {

	return n1 < n2;
}

void _quick_sort(DATATYPE* array, int left, int right) {
	if (left >= right) return;
	DATATYPE key = array[left];
	int i = left, j = right;

	while (i < j) {
		while (i < j && cmp_func(key, array[j])) {
			j--;
		}
		array[i] = array[j];

		while (i < j && cmp_func(array[i], key)) {
			i++;
		}
		array[j] = array[i];
	}
	array[i] = key;

	_quick_sort(array, left, i - 1);
	_quick_sort(array, i + 1, right);
}

//Quicik sort
void QuickSort(DATATYPE* array, int left, int right, CMP_FUNC compare) {
	if (!array || left >= right || left <= 0) return;
	cmp_func = compare ? compare : _default_compare;

	_quick_sort(array, left - 1, right);
}


//test code

void dump_array(DATATYPE *array, int count) {
	for (int i = 0; i < count; i++) {
		printf("%d\t", array[i]);
	}
}

int main(int argc, char** argv) {
	DATATYPE array[] = {555, 123, 666, 444, 21, 65, 44, 244};
	int count = sizeof(array)/sizeof(array[0]);
	printf("count:%d\n", count);

	QuickSort(array, 1, count, 0);

	dump_array(array, count);

	system("pause");
	return 0;
}