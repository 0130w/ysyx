#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/** Our old f**king friend die from ex17. */
void die(const char* message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n",message);
	}

	exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a,int b);	//草 还能这么写

typedef int* (*new_sort)(int *numbers, int count, compare_cb cmp);
/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting
 */

int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int *target = malloc(count * sizeof(int));

	if(!target) {
		die("Memory error.");
	}

	memcpy(target,numbers,count*sizeof(int));

	for(int i = 0; i < count; ++i) {
		for(int j = 0; j < count - 1; ++j) {
			if(cmp(target[j], target[j+1]) > 0) {
				temp = target[j];
				target[j] = target[j+1];
				target[j+1] = temp;
			}
		}
	}

	return target;
}

/**
 * A classic insert sort function that use the
 * compare_cb to do the sorting
 */

int *insert_sort(int *numbers, int count, compare_cb cmp)
{

	int i,j;
	int temp = 0;
	int *target = malloc(count * sizeof(int));

	if(!target) {
		die("Memory error.");
	}

	memcpy(target, numbers, count * sizeof(int));

	for(i = 0; i < count; ++i) {
		temp = target[i];
		j = i-1;
		while((j >= 0) && (cmp(target[j], temp) > 0)) {
			target[j+1] = target[j];
			--j;
		}
		target[j+1] = temp;
	}

	return target;	
}

int sorted_order(int a, int b)
{
	return a - b;
}

int reverse_order(int a, int b)
{
	return b - a;
}

int strange_order(int a, int b)
{
	if(a == 0 || b == 0) {
		return 0;
	} else {
		return a % b;
	}
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out
 */

void test_sorting(int *numbers, int count, compare_cb cmp, new_sort sort_func)
{
	// edit
	unsigned char *data = (unsigned char *)cmp;
	
	for(int i = 0; i < 25 ; ++i) {
		printf("%02x:", data[i]);
	}

	printf("\n");

	int *sorted = sort_func(numbers, count, cmp);

	if(!sorted) {
		die("Failed to sort as requested.");
	}

	for(int i = 0; i < count; ++i) {
		printf("%d ",sorted[i]);
	}

	printf("\n");

	free(sorted);
}

int main(int argc, char* argv[])
{
	if(argc < 2) {
		die("USAGE: ex18 4 3 1 5 6");
	}

	int count = argc - 1; // Number of elements in array
	char **inputs = argv + 1; // Bias
	
	int *numbers = malloc(count * sizeof(int));
	if(!numbers) {
		die("Memory error.");
	}

	for(int i = 0; i < count; ++i) {
		numbers[i] = atoi(inputs[i]);
	}

	test_sorting(numbers, count, sorted_order, bubble_sort);
	test_sorting(numbers, count, reverse_order, bubble_sort);
	test_sorting(numbers, count, strange_order, bubble_sort);
	test_sorting(numbers, count, sorted_order, insert_sort);
	test_sorting(numbers, count, reverse_order, insert_sort);
	test_sorting(numbers, count, strange_order, insert_sort);

	free(numbers);

	return 0;
}
