#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern void map(int64_t *dst, int64_t *src, int64_t n, int64_t(*f)(int64_t));
extern int64_t reduce(int64_t *dst, int64_t *src, int64_t n, int64_t acc_init, int64_t(*f)(int64_t, int64_t));

int64_t map_func1(int64_t n) {
	return 2 * n;
}

int64_t map_func2(int64_t n) {
	return 4 * n + 10;
}

int64_t reduce_func1(int64_t acc, int64_t n) {
	return acc + n;
}

int64_t reduce_func2(int64_t acc, int64_t n) {
	if (n % 2 == 0) {
		return acc - n;
	}
	return acc + n;
}

int64_t main() {
	char opType[100];
	char opFunc[100];
	int64_t n;

	scanf("%s %s %ld", opType, opFunc, &n);

	int64_t *src = malloc(n * sizeof(int64_t));
	int64_t *dst = malloc(n * sizeof(int64_t));


	for (int64_t i = 0; i < n; i++) {
		scanf("%ld", &src[i]);
	}

	if (!strcmp(opType, "map")) {
		if (!strcmp(opFunc, "map_func1")) {
			map(dst, src, n, map_func1);
		} else {
			map(dst, src, n, map_func2);
		}
		for (int64_t i = 0; i < n; i++) {
			printf("%ld ", dst[i]);
		}
		printf("\n");
	} else {
		int64_t res;
		if (!strcmp(opFunc, "reduce_func1")) {
			res = reduce(dst, src, n, 5, reduce_func1);
		} else {
			res = reduce(dst, src, n, 0, reduce_func2);
		}
		printf("%ld\n", res);
	}

	return 0;

}
