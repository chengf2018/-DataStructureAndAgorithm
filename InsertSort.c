// insert sort
#include <stdio.h>
#include <stdlib.h>

typedef int DATATYPE;
typedef int(*CMP_FUNC)(DATATYPE, DATATYPE);

static CMP_FUNC cmp_func;

int DefaultCompareFunc(DATATYPE d1, DATATYPE d2) {
	return d1 < d2;
}

void InsertSort(DATATYPE *array, int left, int right, CMP_FUNC compare) {
	if (!array || left < 0 || left >= right) return;
	cmp_func = compare ? compare : DefaultCompareFunc;

	for (int i = left; i < right; i++) {
		DATATYPE temp = array[i];
		int j;
		for (j = i; j > left && cmp_func(temp, array[j-1]); j--) {
			array[j] = array[j-1];
		}
		array[j] = temp;
	}
}


void dump_array(DATATYPE *array, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d\t", array[i]);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	DATATYPE array[] = {123, 654, 321, 545, 324, 321, 235, 211, 564, 111};
	int size = sizeof(array)/sizeof(*array);
	printf("%d\n", size);
	InsertSort(array, 0, size, 0);
	dump_array(array, size);
	system("pause");
	return 0;
}