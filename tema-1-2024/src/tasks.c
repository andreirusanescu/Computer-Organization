#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void reverse_int(void *data1, void *data2)
{
	array_t *list = (array_t *)data1;

	/* loading the data from the end to the beginning
	   list.len is used as if an index in an array */

	memcpy((*list).data + ((*list).len - 1) * (*list).elem_size,
		   data2, (*list).elem_size);
	(*list).len--;
}

array_t reverse(array_t list) {
	array_t new_list;
	new_list.destructor = list.destructor;
	new_list.elem_size = list.elem_size;
	new_list.len = list.len;
	new_list.data = malloc(new_list.elem_size * new_list.len);
	reduce(reverse_int, &new_list, list);
	new_list.len = list.len;
	return new_list;
}

void init_dec(void *data1, void *data2)
{
	array_t *list = (array_t *)data1;
	memcpy(&(*(number_t *)((*list).data +
		  (*list).len * (*list).elem_size)).integer_part, data2, sizeof(int));
	(*list).len++;
}

void init_frac(void *data1, void *data2)
{
	array_t *list = (array_t *)data1;

	/* loading the data with (*list).len as if it would load
	   list.data[i]. list.len is incremented very iteration */
	memcpy(&(*(number_t *)((*list).data + (*list).len *
		  (*list).elem_size)).fractional_part, data2, sizeof(int));
	(*list).len++;
}

void init_float(void *data1, void *data2)
{
	array_t *list = (array_t *)data1;
	int n1 = (*(number_t *)((*list).data +
			 (*list).len * (*list).elem_size)).integer_part;
	int n2 = (*(number_t *)((*list).data +
			 (*list).len * (*list).elem_size)).fractional_part;
	char *s = (char *)calloc(50, 1);

	/* gets the string of a number */
	sprintf(s, "%d.%d", n1, n2);
	(*(number_t *)((*list).data +
	(*list).len * (*list).elem_size)).string = strdup(s);
	free(s);
	(*list).len++;
	(void)data2;
}

static void number_destructor(void *elem)
{
	free(((number_t *)elem)->string);
}

array_t create_number_array(array_t integer_part, array_t fractional_part) {
	array_t new_list;
	new_list.elem_size = sizeof(number_t);

	/* new_list.len is used for iterating through each function
	   passed to reduce, it is reset every time reduce is called;
	   loading the new_list.data in 3 steps, first the integer,
	   fractional, then the string */

	new_list.len = 0;
	new_list.data = malloc(integer_part.len * sizeof(number_t));
	reduce(init_dec, &new_list, integer_part);
	new_list.len = 0;
	reduce(init_frac, &new_list, fractional_part);
	new_list.len = 0;
	reduce(init_float, &new_list, integer_part);
	new_list.len = integer_part.len;
	new_list.destructor = number_destructor;
	free(integer_part.data);
	free(fractional_part.data);
	return new_list;
}

boolean check_pass(void *data)
{
	student_t stud = *(student_t *)data;
	if (stud.grade > 5.0)
		return true;
	return false;
}

static void name_destructor(void *data)
{
	if (*(char **)data)
		free(*(char **)data);
}

void make_stud(void *data1, void *data2)
{
	student_t stud = *(student_t *)data2;
	char **name = (char **)data1;
	*name = strdup(stud.name);
	(void)data1;
}

array_t get_passing_students_names(array_t list) {
	/* removing the students that failed */
	array_t filtered_list = filter(check_pass, list);

	/* holding just the names of the students that passed */
	array_t new_list = map(make_stud, sizeof(char *),
						   name_destructor, filtered_list);
	return new_list;
}

void find_sum(void *data1, void *data2)
{
	*(int *)data1 += *(int *)data2;
}

void convert_sum(void *data1, void *data2)
{
	array_t list = *(array_t *)data2;
	reduce(find_sum, data1, list);
}

void make_bool(void *data1, void **data2)
{
	boolean *ok = (boolean *)data1;
	int value1 = *(int *)data2[0];
	int value2 = *(int *)data2[1];

	/* checking the condition proposed */
	if (value1 >= value2)
		*ok = 1;
	else
		*ok = 0;
}

array_t check_bigger_sum(array_t list_list, array_t int_list) {
	/* list is a list of the sums of each list from list_list*/
	array_t list = map(convert_sum, sizeof(int), NULL, list_list);

	/* aux is the list of booleans */
	array_t aux = map_multiple(make_bool, sizeof(boolean),
							   NULL, 2, list, int_list);
	return aux;
}

void indexed(void *data1, void *data2)
{
	array_t *list = (array_t *)data1;

	/* if it's an even indexed string it is added
	   otherwise the memory is freed*/

	if ((*list).len % 2 == 0) {
		memcpy((*list).data + ((*list).len / 2) * (*list).elem_size,
		       data2, (*list).elem_size);
	} else {
		free(*(char **)data2);
	}
	(*list).len++;
}

array_t get_even_indexed_strings(array_t list) {
	array_t new_list;
	new_list.destructor = list.destructor;
	new_list.elem_size = list.elem_size;

	/* new_list.len is used for loading each element
	   at the right index */
	new_list.len = 0;

	/* there will be exactly list.len / 2 + (list.len % 2)
	   strings in the new_list */
	new_list.data = calloc((list.len / 2 + (list.len % 2)), sizeof(char *));
	reduce(indexed, &new_list, list);

	// assigning the right value
	new_list.len = list.len / 2 + (list.len % 2);
	free(list.data);
	return new_list;
}

static void matrix_destructor(void *data)
{
	free(*(array_t **)data);
}

void make_row(void *data1, void *data2)
{
	array_t *row = (array_t *)data1;
	int val = (*row).elem_size + 1;

	// adding at row[index]
	memcpy((*row).data + (*row).len * sizeof(int), &val, sizeof(int));
	(*row).len++;
	(*row).elem_size++;
	(void)data2;
}

void make_matrix(void *data1, void *data2)
{
	array_t *matrix = (array_t *)data1;

	array_t row;
	row.elem_size = 0 + (*matrix).elem_size;
	row.len = 0;
	row.data = calloc((*matrix).len, sizeof(int));

	/* Using matrix for it's length to iterate
	   through make_row. Row_len is used for
	   loading up the elements like row[0], row[1]..
	   and row.elem_size is used for the value that
	   is loaded up*/
	reduce(make_row, &row, *matrix);

	/* assigning the right values for the row */
	row.len = (*matrix).len;
	row.elem_size = sizeof(int);

	/* loading up the row in the matrix */
	memcpy((*matrix).data + (*matrix).elem_size * sizeof(array_t),
	       &row, sizeof(array_t));

	(*matrix).elem_size++;
	(void)data2;
}

array_t generate_square_matrix(int n) {
	array_t matrix;

	/* matrix.elem_size helps load the rows in matrix
	   and find the corresponding values; matrix[0], matrix[1]
	   are basically the rows in the matrix, and elem_size is
	   the index
	   Initially set to 0, at each iteration through make_matrix
	   it is incremented by one; */

	matrix.elem_size = 0;
	matrix.len = n;
	matrix.destructor = matrix_destructor;
	matrix.data = malloc(n * sizeof(array_t));

	/* matrix that helps with the for loop in reduce;
	   Only used for its length so it iterates n times */
	array_t aux;
	aux.elem_size = 0;
	aux.len = n;

	/* the accumulator is the matrix;
	   the programme is loading it as it runs */
	reduce(make_matrix, &matrix, aux);

	// assigning the right value;
	matrix.elem_size = sizeof(array_t);
	return matrix;
}
