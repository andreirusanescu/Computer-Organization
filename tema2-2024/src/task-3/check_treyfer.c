#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define BLOCK_SIZE 8

extern uint8_t sbox[256];
extern uint32_t num_rounds;
extern void treyfer_crypt(uint8_t *text, uint8_t *key);
extern void treyfer_dcrypt(uint8_t *text, uint8_t *key);

size_t get_file_size(FILE *f) {
	off_t cur = ftell(f);
	fseek(f, 0, SEEK_END);
	size_t siz = ftell(f);
	fseek(f, cur, SEEK_SET);
	return siz;
}

double do_tests(int type, int num_tests) {
	double score = 0.0;
	char input_file[256] = {0};
	char key_file[256] = {0};
	char ref_file[256] = {0};
	char out_file[256] = {0};
	FILE *in, *key, *ref, *out;

	if (type == 0) {
		printf("------------ENCRYPT TESTS--------------\n\n");
	} else {
		printf("------------DECRYPT TESTS--------------\n\n");
	}


    	for (int i = 0; i < num_tests; i++) {
		if (type == 0) {
			sprintf(input_file, "input/treyfer_enc_%d.in", i);
			in = fopen(input_file, "rb");

			sprintf(key_file, "input/treyfer_enc_%d.key", i);
			key = fopen(key_file, "rb");

			sprintf(ref_file, "ref/treyfer_enc_%d.ref", i);
			ref = fopen(ref_file, "rb");

			sprintf(out_file, "output/treyfer_enc_%d.out", i);
			out = fopen(out_file, "wb");

		} else {
			sprintf(input_file, "input/treyfer_dec_%d.in", i);
			in = fopen(input_file, "rb");

			sprintf(key_file, "input/treyfer_dec_%d.key", i);
			key = fopen(key_file, "rb");

			sprintf(ref_file, "ref/treyfer_dec_%d.ref", i);
			ref = fopen(ref_file, "rb");

			sprintf(out_file, "output/treyfer_dec_%d.out", i);
			out = fopen(out_file, "wb");
		}


		char in_block[BLOCK_SIZE];
		char ref_block[BLOCK_SIZE];
		char key_bytes[BLOCK_SIZE];
		fread(key_bytes, 1, BLOCK_SIZE, key);

		size_t siz = get_file_size(ref);
		int ok = 1;

		for (int j = 0; j < siz; j += 8) {
			fread(in_block, 1, BLOCK_SIZE, in);
			fread(ref_block, 1, BLOCK_SIZE, ref);
			if (type == 0) {
				treyfer_crypt(in_block, key_bytes);
			} else {
				treyfer_dcrypt(in_block, key_bytes);
			}

			fwrite(in_block, 1, 8, out);

			if (memcmp(in_block, ref_block, BLOCK_SIZE)) {
				ok = 0;
				break;
			}
		}

		if (ok) {
			score += 2.5f;
			printf("Test %d..................PASSED: %.2lfp\n", i + 1, (double)2.5);
		} else {
			printf("Test %d..................FAILED: %.2lfp\n", i + 1, (double)0);
		}

		fclose(in);
		fclose(key);
		fclose(ref);
		fclose(out);

	}
	printf("\n");
	return score;
}


int main() {
	double score = 0.0;
	printf("--------------TASK 3-------------------\n\n");
	score += do_tests(0, 5);
	score += do_tests(1, 5);
    	printf("\nTASK 3 SCORE: %.2lf / 25.00\n\n", score);

	return 0;
}
