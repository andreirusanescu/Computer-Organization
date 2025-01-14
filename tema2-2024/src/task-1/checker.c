#include <stdio.h>

#include "constants.h"

void check_permission(unsigned int n, unsigned int* res);

unsigned int readInput(char *filename)
{
    FILE *input = fopen(filename, "r");
    int input_number = 0, room;

	// read the id of the user
    fscanf(input, "%d\n", &input_number);
	input_number = input_number << 24;

	// read the rooms that he wants access to
	while (fscanf(input, "%d\n", &room) != -1) 
		input_number = input_number | (1 << (room));
	
    fclose(input);

    return input_number;
}

int readRef(char *filename)
{
    FILE *ref = fopen(filename, "r");
	int ref_num;

	// read the id of the user
	fscanf(ref, "%d\n", &ref_num);
   
    fclose(ref);

	return ref_num;
}

void printOutput(char *filename, int res)
{
    FILE *output = fopen(filename, "w");

    fprintf(output, "%d\n", res);

    fclose(output);
}



int main(int argc, char **argv) {
	float score = 0;
	unsigned int x, ref;
	char input_file[30], output_file[30], ref_file[30];

	printf("--------------TASK 1--------------\n");

	for (int i = 0; i < NO_TASKS; i++) {
		/* read input */
		sprintf(input_file, "./input/permission_%d.in", i + 1);
		sprintf(ref_file, "./ref/permission_%d.ref", i + 1);

		x = readInput(input_file);

		ref = readRef(ref_file);

		unsigned int res = 2;
		check_permission(x, &res);

		if (res == ref) {
			printf("Test %02d.................PASSED: %.1fp\n", i + 1, 1.5);
			score += 1.5;
		} 
		else {
			printf("Test %02d.................FAILED: %.1fp\n", i + 1, 0.0);
		}

		sprintf(output_file, "./output/permission_%d.out", i + 1);
		printOutput(output_file, res);
		
	}
	printf("\nTASK 1 SCORE: %.2f / 15.00\n\n", score);

	return 0;
}