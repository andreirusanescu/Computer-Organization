#include "functional.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void for_each(void (*func)(void *), array_t list)
{
	for (int i = 0; i < list.len; ++i)
		func(list.data + i * list.elem_size);
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list)
{
	array_t new_list;
	new_list.destructor = new_list_destructor;
	new_list.elem_size = new_list_elem_size;
	new_list.len = list.len;
	new_list.data = calloc(new_list.len, new_list_elem_size);
	for (int i = 0; i < list.len; ++i)
		func(new_list.data + i * new_list.elem_size,
			 list.data + i * list.elem_size);

	if (list.destructor)
		for_each(list.destructor, list);
	free(list.data);
	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list)
{
	array_t new_list;
	new_list.destructor = list.destructor;
	new_list.elem_size = list.elem_size;
	int cnt = 0;
	for (int i = 0; i < list.len; ++i)
		if (func(list.data + i * list.elem_size))
			cnt++;
	new_list.len = cnt;
	new_list.data = calloc(cnt, list.elem_size);
	int j = 0;
	for (int i = 0; i < list.len; ++i) {
		if (func(list.data + i * list.elem_size)) {
			memcpy(new_list.data + j * list.elem_size,
			       list.data + i * list.elem_size, list.elem_size);
			j++;
		} else {
			if (list.destructor)
				list.destructor(list.data + i * list.elem_size);
		}
	}

	free(list.data);
	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list)
{
	for (int i = 0; i < list.len; ++i)
		func(acc, list.data + i * list.elem_size);
	return acc;
}

void for_each_multiple(void(*func)(void **), int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);

	/* loading the arguments in an array */
	array_t *array = (array_t *)malloc(varg_c * sizeof(array_t));
	int min;
	for (int i = 0; i < varg_c; ++i) {
		array[i] = va_arg(args, array_t);
		if (!i || array[i].len < min)
			min = array[i].len;
	}
	va_end(args);
	void **values = malloc(varg_c * sizeof(void *));
	for (int i = 0; i < min; ++i) {
		for (int j = 0; j < varg_c; ++j) {
			values[j] = malloc(array[j].elem_size);
			/* extracting the column array */
			memcpy(values[j], array[j].data + i * array[j].elem_size,
				   array[j].elem_size);
		}
		func(values);
		for (int j = 0; j < varg_c; ++j)
			free(values[j]);
	}
	free(values);
	free(array);
}

array_t map_multiple(void (*func)(void *, void **),
					 int new_list_elem_size,
					 void (*new_list_destructor)(void *),
					 int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);

	// loading args;
	array_t *arr = (array_t *)malloc(varg_c * sizeof(array_t));
	int min;
	for (int i = 0; i < varg_c; ++i) {
		arr[i] = va_arg(args, array_t);
		if (!i || arr[i].len < min)
			min = arr[i].len;
	}
	va_end(args);

	// initialise new list;
	array_t new_list;
	new_list.destructor = new_list_destructor;
	new_list.elem_size = new_list_elem_size;
	new_list.len = min;
	new_list.data = malloc(min * new_list.elem_size);

	void **values = malloc(varg_c * sizeof(void *));
	for (int i = 0; i < min; ++i) {
		for (int j = 0; j < varg_c; ++j) {
			values[j] = malloc(arr[j].elem_size);
			memcpy(values[j], arr[j].data + i * arr[j].elem_size,
				   arr[j].elem_size);
		}
		func(new_list.data + i * new_list.elem_size, values);
		for (int j = 0; j < varg_c; ++j)
			free(values[j]);
	}
	free(values);

	// free old lists;
	for (int i = 0; i < varg_c; ++i) {
		/* freeing the memory for the old lists */
		if (arr[i].destructor)
			for_each(arr[i].destructor, arr[i]);
		free(arr[i].data);
	}
	free(arr);
	return new_list;
}

void *reduce_multiple(void(*func)(void *, void **), void *acc, int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);

	array_t *array = (array_t *)malloc(varg_c * sizeof(array_t));
	int min;
	for (int i = 0; i < varg_c; ++i) {
		array[i] = va_arg(args, array_t);
		if (!i || array[i].len < min)
			min = array[i].len;
	}
	va_end(args);
	for (int i = 0; i < min; ++i) {
		void **values = malloc(varg_c * sizeof(void *));
		for (int j = 0; j < varg_c; ++j) {
			values[j] = malloc(array[j].elem_size);
			memcpy(values[j], array[j].data + i * array[j].elem_size,
				   array[j].elem_size);
		}
		func(acc, values);
		for (int j = 0; j < varg_c; ++j)
			free(values[j]);
		free(values);
	}
	free(array);
	return acc;
}
