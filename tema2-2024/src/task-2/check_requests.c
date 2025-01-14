#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct creds {
    unsigned short passkey;
    char username[51];
} __attribute__((packed));

struct request {
    unsigned char admin;
    unsigned char prio;
    struct creds login_creds;
} __attribute__((packed));

int tests_num, types_num;
float *type_score;

void sort_requests(struct request *requests, int len);
void check_passkeys(struct request *requests, int len, char *connected);

void readConfig(char *filename)
{
    FILE *config = fopen(filename, "r");

    fscanf(config, "tests_num: %d\n", &tests_num);
    fscanf(config, "types: %d\n", &types_num);
    type_score = malloc(types_num * sizeof(float));

    char *test_string = malloc(40);

    for (int i = 0; i < types_num; i++) {
        sprintf(test_string, "type %d: ", i + 1);
        fscanf(config, strcat(test_string, "%f\n"), &type_score[i]);
    }
}

int readInput(char *filename, struct request **requests_arr, char **connected)
{
    FILE *input = fopen(filename, "r");
    int len;

    /* read requests */
    fscanf(input, "%d\n", &len);
    *requests_arr = malloc(len * sizeof(struct request));
    *connected = malloc(len * sizeof(**connected));
    for(int i = 0 ; i < len ; i++) {
        fscanf(input, "%hhu %hhu %hu %s\n", &(*requests_arr)[i].admin,
                                           &(*requests_arr)[i].prio,
                                           &(*requests_arr)[i].login_creds.passkey,
                                           (*requests_arr)[i].login_creds.username);
    }
    fclose(input);

    return len;
}

void readRef(char *filename, int len, struct request *ref_requests, char *connected)
{
    FILE *ref = fopen(filename, "r");

    for(int i = 0 ; i < len; i++) {
        fscanf(ref, "%hhu %hhu %hu %s\n", &ref_requests[i].admin,
                                         &ref_requests[i].prio,
                                         &ref_requests[i].login_creds.passkey,
                                         ref_requests[i].login_creds.username);
    }

    for (int i = 0; i < len; i++) {
        fscanf(ref, "%hhd\n", &connected[i]);
    }
    fclose(ref);
}

void printOutput(char *filename, int len, struct request *out_requests, char *connected)
{
    FILE *output = fopen(filename, "w");

    for (int i = 0; i < len; i++) {
        fprintf(output, "%hhu %hhu %hu %s\n", out_requests[i].admin,
                                              out_requests[i].prio,
                                              out_requests[i].login_creds.passkey,
                                              out_requests[i].login_creds.username);
    }

    for (int i = 0; i < len; i++) {
        fprintf(output, "%hhd\n", connected[i]);
    }

    fclose(output);
}

void printScore(float *sort_tests_score, float *run_tests_score)
{
    float score = 0.0;

    printf("-------------SORT TESTS-------------\n\n");
    for (int i = 0; i < tests_num; i++) {
        if (sort_tests_score[i]) {
            if (i + 1 < 10) {
                printf("Test %d..................PASSED: %.2fp\n", i + 1, sort_tests_score[i]);
            } else {
                printf("Test %d.................PASSED: %.2fp\n", i + 1, sort_tests_score[i]);
            }
            score += sort_tests_score[i];
        } else {
            if (i + 1 < 10) {
                printf("Test %d..................FAILED: %.2fp\n", i + 1, 0.0);
            } else {
                printf("Test %d.................FAILED: %.2fp\n", i + 1, 0.0);
            }
        }
    }
    printf("\n");

    printf("------------HACKER TESTS--------------\n\n");
    for (int i = 0; i < tests_num; i++) {
        if (run_tests_score[i]) {
            if (i + 1 < 10) {
                printf("Test %d..................PASSED: %.2fp\n", i + 1, run_tests_score[i]);
            } else {
                printf("Test %d.................PASSED: %.2fp\n", i + 1, run_tests_score[i]);
            }
            score += run_tests_score[i];
        } else {
            if (i + 1 < 10) {
                printf("Test %d..................FAILED: %.2fp\n", i + 1, 0.0);
            } else {
                printf("Test %d.................FAILED: %.2fp\n", i + 1, 0.0);
            }
        }
    }

    printf("\nTASK 2 SCORE: %.2f / 20.00\n", score);
}

int compare_structs(struct request *first, struct request *second)
{
    int username_cmp = strcmp(first->login_creds.username,
                              second->login_creds.username);

    if (first->prio != second->prio
        || first->admin != second->admin
        || first->login_creds.passkey != second->login_creds.passkey
        || username_cmp != 0) {
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int len = 0;
    float score = 0;

    readConfig(".config");
    float sort_tests_score[tests_num], run_tests_score[tests_num];

    struct request *requests = NULL, *ref_requests = NULL;
    char *connected = NULL, *ref_connected = NULL;
    char input_file[30], output_file[30], ref_file[30];

    printf("---------------TASK 2---------------\n\n");
    for (int i = 0; i < 10; i++) {
        /* read input */
        sprintf(input_file, "./input/requests_%d.in", i + 1);
        len = readInput(input_file, &requests, &connected);

        /* read ref */
        sprintf(ref_file, "./ref/requests_%d.ref", i + 1);
        ref_requests = malloc(len * sizeof(*ref_requests));
        ref_connected = malloc(len * sizeof(*ref_connected));
        readRef(ref_file, len, ref_requests, ref_connected);

        sort_requests(requests, len);
        check_passkeys(requests, len, connected);

        int sort_ok = 1, check_passkeys_ok = 1;

        for (int j = 0; j < len; j++) {
            if (compare_structs(&requests[j], &ref_requests[j])) {
                sort_ok = 0;
            }
        }

        sort_tests_score[i] = sort_ok ? type_score[0] : 0;

        for (int j = 0; j < len; j++) {
            if (connected[j] != ref_connected[j]) {
                check_passkeys_ok = 0;
            }
        }

        run_tests_score[i] = check_passkeys_ok ? type_score[1] : 0;

        sprintf(output_file, "./output/requests_%d.out", i + 1);
        printOutput(output_file, len, requests, connected);
    }

    printScore(sort_tests_score, run_tests_score);

    return 0;
}
