#include <stdio.h>
#include <random>
#include <time.h>

bool valid(const double* array, int lenght) {
	for (int i = 0; i < lenght - 1; i++) {
		if (array[i] > array[i + 1])
			return 0;
	}
	return 1;
}

int brush_sort(double* array, int lenght) {
	int operationCount = 0;
	int step = lenght / 2;
	for (; step > 1; step /= 2) {
		for (int i = 0; i < step; i++) {
			for (int j = i; j + step < lenght; j += step) {
				if (array[j] > array[j + step]) {
					double t = array[j];
					array[j] = array[j + step];
					array[j + step] = t;
					++operationCount;
				}
			}
		}
	}
	if (!valid(array, lenght)) {
		for (int i = 0; i < lenght - 1; i++) {
			for (int j = 0; j < lenght - i - 1; j++) {
				if (array[j] > array[j + 1]) {
					double t = array[j];
					array[j] = array[j + 1];
					array[j + 1] = t;
					operationCount++;
				}
			}
		}
	}
  return operationCount;
}

int selection_sort(double* array, int lenght) {
	int operationCount = 0;
	for (int i = 0; i < lenght - 1; i++) {
		int min = i;
		for (int j = i + 1; j < lenght; j++) {
			if (array[j] < array[min]) {
				min = j;
				operationCount++;
			}
		}
		double t = array[i];
		array[i] = array[min];
		array[min] = t;
	}
  return operationCount;
}

void merge(double* array, int left, int mid, int right, int* operationCount) {
	double* lArray = (double*)malloc(sizeof(double) * (mid - left + 1));
	double* rArray = (double*)malloc(sizeof(double) * (right - mid));
  for (int i = 0; i < mid - left + 1; i++)
    lArray[i] = array[left + i];
	for (int i = 0; i < right - mid; i++)
		rArray[i] = array[mid + 1 + i];
	int lIndex = 0, rIndex = 0, mIndex = left;
	while (lIndex < mid - left + 1 && rIndex < right - mid) {
		if (lArray[lIndex] < rArray[rIndex]) {
      array[mIndex] = lArray[lIndex];
			lIndex++;
			(*operationCount)++;
		}
		else {
			array[mIndex] = rArray[rIndex];
			rIndex++;
			(*operationCount)++;
		}
		mIndex++;
	}
  while (lIndex < mid - left + 1) {
    array[mIndex] = lArray[lIndex];
    lIndex++;
    mIndex++;
		(*operationCount)++;
  }
  while (rIndex < right - mid) {
    array[mIndex] = rArray[rIndex];
    rIndex++;
    mIndex++;
		(*operationCount)++;
  }
  free(lArray);
	free(rArray);
}
void merge_sort(double* array, int left, int right, int* operationCount) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort(array, left, mid, operationCount);
		merge_sort(array, mid + 1, right, operationCount);
		merge(array, left, mid, right, operationCount);
	}
}

void count(double mas[], int counter[], int lenght, int offset) {
	char* b = (char*)mas;
	b = b + offset;
	memset(counter, 0, 256 * sizeof(int));
	for (int i = 0; i < lenght; i++) {
		counter[(unsigned char)*b]++;
		b += sizeof(double);
	}
	int s = 0, tmp;
	for (int i = 0; i < 256; i++) {
		tmp = counter[i];
		counter[i] = s;
		s += tmp;
	}
}
int radix_sort(double* array, int lenght) {
	int operationCount = 0;
	int counter[256];
	double* b = (double*)malloc(lenght * sizeof(double));
	char* c = (char*)array;
	for (int i = 0; i < sizeof(double); i++) {
		count(array, counter, lenght, i);
		for (int j = 0; j < lenght; j++) {
			operationCount++;
			b[counter[(unsigned char)*(c + i + j * sizeof(double))]++] = array[j];
		}
		for (int j = 0; j < lenght; j++) {
			operationCount++;
			array[j] = b[j];
		}
	}
	int index;
	for (index = 0; index < lenght; index++) {
		if (b[index] < 0.0) {
			operationCount++;
			break;
		}
	}
	for (int i = 0; i < lenght - index; i++) {
		operationCount++;
		array[i] = b[lenght - 1 - i];
	}

	for (int i = lenght - index; i < lenght; i++) {
		operationCount++;
		array[i] = b[i - lenght + index];
	}
	free(b);
  return operationCount;
}

int main() {
	srand(time(0));
	int lenght, operation;
	printf("Enter array size: ");
	scanf_s("%d", &lenght);
	clock_t start, end;
	double* array = (double*)malloc(sizeof(double) * lenght);
	
	for (int i = 0; i < lenght; ++i) {
		array[i] = (double)rand() / (double)(RAND_MAX / 100);
	}
  start = clock();
	operation = brush_sort(array, lenght);
	end = clock();
  printf("\nThe brush sorting is done correctly: %d, operation count: %d, time: %f\n", valid(array, lenght), operation, (float)(end - start) / CLOCKS_PER_SEC);

	for (int i = 0; i < lenght; ++i) {
		array[i] = (double)rand() / (double)(RAND_MAX / 100);
	}
	start = clock();
	operation = selection_sort(array, lenght);
	end = clock();
	printf("\nThe selection sorting is done correctly: %d, operation count: %d, time: %f\n", valid(array, lenght), operation, (float)(end - start) / CLOCKS_PER_SEC);

	for (int i = 0; i < lenght; ++i) {
		array[i] = (double)rand() / (double)(RAND_MAX / 100);
	}
	start = clock();
	operation = 0;
	merge_sort(array, 0, lenght, &operation);
	end = clock();
	printf("\nThe merge sorting is done correctly: %d, operation count: %d, time: %f\n", valid(array, lenght), operation, (float)(end - start) / CLOCKS_PER_SEC);

	for (int i = 0; i < lenght; ++i) {
		array[i] = (double)rand() / (double)(RAND_MAX / 100);
	}
	start = clock();
	operation = radix_sort(array, lenght);
	end = clock();
	printf("\nThe radix sorting is done correctly: %d, operation count: %d, time: %f\n", valid(array, lenght), operation, (float)(end - start) / CLOCKS_PER_SEC);
}