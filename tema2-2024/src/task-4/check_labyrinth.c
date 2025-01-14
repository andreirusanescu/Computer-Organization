#include <stdio.h>
#include <stdlib.h>

#define N_TESTS 10

typedef struct position {
    unsigned int line;
    unsigned int col;
} Position;


void solve_labyrinth(unsigned int *out_line, unsigned int *out_col,
                        unsigned int m, unsigned int n, char **labyrinth);

void read_test(unsigned int test_no, char ***a, 
                unsigned int *m, unsigned int *n) {
    unsigned int line, col;
    FILE *file;
    char file_name[30];

    sprintf(file_name, "./input/labyrinth_%d.in", test_no);

    file = fopen(file_name, "r");

    fscanf(file, "%d %d", m, n);
    fgetc(file);
    *a = (char **) malloc(*m * sizeof(**a));

    for (line = 0; line < *m; line++) {
        (*a)[line] = (char *) malloc(*n * sizeof(***a));
        for (col = 0; col < *n; col++) {
            fscanf(file, "%c", &((*a)[line][col]));
            fgetc(file);
        }
    }

    fclose(file);
}


void free_test(unsigned int m, char **a) {
    unsigned int line;

    for (line = 0; line < m; line++) {
        free(a[line]);
    }
    free(a);
}


void check_result(int test_no, Position *sol, double *score) {
    char ref_filename[30], output_filename[30];
    FILE *ref_file,*output_file;
    Position correct_solution;

    sprintf(ref_filename, "./ref/labyrinth_%d.ref", test_no);
    ref_file = fopen(ref_filename, "r");

    fscanf(ref_file, "%u %u", &correct_solution.line, &correct_solution.col);
    printf("%d %d\n", correct_solution.line, correct_solution.col);
    printf("%d %d\n", sol->line, sol->col);
    if (correct_solution.line == sol->line 
        && correct_solution.col == sol->col) {
        printf("Test %d.................PASSED: %dp\n", test_no, 3);
        *score += 3.0;
    } else {
        printf("Test %d.................FAILED: %dp\n", test_no, 0);
    }

    fclose(ref_file);

    sprintf(output_filename, "./output/labyrinth_%d.out", test_no);
    output_file = fopen(output_filename, "w");
    fprintf(output_file, "%u %u\n", correct_solution.line, correct_solution.col);
    fclose(output_file);
}


int main() {
    Position sol;
    unsigned int m, n, test_no, line, col;
    char **a;
    double score = 0.0;

    printf("--------------TASK 4--------------\n");

    for (test_no = 0; test_no < N_TESTS; test_no++) {
        read_test(test_no, &a, &m, &n);

        solve_labyrinth(&sol.line, &sol.col, m, n, a);

        check_result(test_no, &sol, &score);

        free_test(m, a);
    }
    printf("\nTASK 4 SCORE: %.2f / 30.00\n\n", score);


    return 0;
}
