#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define NUM_TESTS 10        /* tests are numbered 1..10 */

/* asm implementations required */
void quick_sort(int32_t *buff, uint32_t start, uint32_t end);

int32_t __attribute__((fastcall))
binary_search(int32_t *buff, uint32_t needle, uint32_t start, uint32_t end);


/* get_input - reads contents of input file into a buffer
 *  @path   : input file path
 *  @len    : ptr to variable where input length is stored
 *  @needle : ptr to varuable where searched value is stored
 *            (ignored if NULL)
 *
 *  @return : ptr to array filled with input values
 *
 * NOTE: returned buffer must be freed by caller
 */
int32_t *
get_input(char *path, uint32_t *len, int32_t *needle)
{
    FILE    *fin;
    int32_t *buff = NULL;

    /* open input file */
    fin = fopen(path, "r");
    if (!fin) {
        printf("unable to open \"%s\" (%s)\n", path, strerror(errno));
        return NULL;
    }

    /* allocate buffer for all elements */
    fscanf(fin, "%u", len);
    buff = malloc(*len * sizeof(int32_t));
    if (!buff) {
        printf("unable to allocate buffer (%s)\n", strerror(errno));
        goto out_close;
    }

    /* (optional) read searched value for bsearch subtask */
    if (needle)
        fscanf(fin, "%d", needle);

    /* read elements */
    for (size_t i = 0; i < *len; i++)
        fscanf(fin, "%d", &buff[i]);

    /* cleanup sequence */
out_close:
    fclose(fin);

    return buff;
}


/* get_reference - reads contents of reference file into a buffer
 *  @path : reference file path
 *  @len  : number of elements in the reference file (known from input file)
 *
 *  @return : ptr to array filled with reference values
 *
 * NOTE: returned buffer must be freed by caller
 */
int32_t *
get_reference(char *path, uint32_t len)
{
    FILE *fin;
    int32_t *buff = NULL;

    /* open input file */
    fin = fopen(path, "r");
    if (!fin) {
        printf("unable to open \"%s\" (%s)\n", path, strerror(errno));
        return NULL;
    }

    /* allocate buffer for all elements */
    buff = malloc(len * sizeof(int32_t));
    if (!buff) {
        printf("unable to allocate buffer (%s)\n", strerror(errno));
        goto out_close;
    }

    /* read a pre-determined number of elements */
    for (size_t i = 0; i < len; i++)
        fscanf(fin, "%d", &buff[i]);

out_close:
    fclose(fin);

    return buff;
}


/* dump_output - dumps array to file in human-readable format
 *  @path : output file path
 *  @buff : array to dump
 *  @len  : array length
 */
void
dump_output(char *path, int32_t *buff, uint32_t len)
{
    FILE *fout;

    /* sanity checks */
    if (!buff || !len)
        return;

    /* open output file */
    fout = fopen(path, "w");
    if (!fout) {
        printf("unable to open \"%s\" (%s)", path, strerror(errno));
        return;
    }

    /* write array contents */
    for (size_t i = 0; i < len - 1; i++)
        fprintf(fout, "%d ", buff[i]);
    fprintf(fout, "%d\n", buff[len - 1]);

    /* cleanup */
    fclose(fout);
}


/* run_qsort_test - executes a specific qsort test
 *  @num : test number (starts from 1)
 *
 *  @return : 1 if successful; 0 otherwise
 */
uint32_t
run_qsort_test(uint32_t num)
{
    char     path[100];     /* file path        */
    uint32_t len;           /* input length     */
    int32_t  *buff;         /* input array      */
    int32_t  *ref_buff;     /* reference array  */
    uint32_t ret = 0;       /* return value     */

    /* read input file */
    snprintf(path, sizeof(path), "input/qsort_%u.in", num);
    buff = get_input(path, &len, NULL);
    if (!buff)
        return 0;

    /* read reference file */
    snprintf(path, sizeof(path), "ref/qsort_%u.ref", num);
    ref_buff = get_reference(path, len);
    if (!ref_buff)
        goto out_free;

    /* sort array */
    quick_sort(buff, 0, len - 1);

    /* compare output with reference */
    for (size_t i = 0; i < len; i++)
        if (buff[i] != ref_buff[i])
            goto out_dump;

    /* success */
    ret = 1;

out_dump:
    /* dump sorted array to file */
    snprintf(path, sizeof(path), "output/qsort_%u.out", num);
    dump_output(path, buff, len);

out_free:
    /* buffer cleanup */
    free(ref_buff);
    free(buff);

    return ret;
}


/* run_bsearch_test - executes a specific bsearch test
 *  @num : test number (starts from 1)
 *
 *  @return : 1 if successful; 0 otherwise
 */
uint32_t
run_bsearch_test(uint32_t num)
{
    char     path[100];     /* file path        */
    uint32_t len;           /* input length     */
    int32_t  needle;        /* searched value   */
    int32_t  idx;           /* needle's index   */
    int32_t  *buff;         /* input array      */
    int32_t  *ref;          /* reference output */
    uint32_t ret = 0;       /* return value     */

    /* read input file */
    snprintf(path, sizeof(path), "input/bsearch_%u.in", num);
    buff = get_input(path, &len, &needle);
    if (!buff)
        return 0;

    /* read reference file */
    snprintf(path, sizeof(path), "ref/bsearch_%u.ref", num);
    ref = get_reference(path, 1);
    if (!ref)
        goto out_free;

    /* perform search */
    idx = binary_search(buff, needle, 0, len - 1);

    /* compare output with reference */
    ret = (idx == *ref);

out_dump:
    /* dump sorted array to file */
    snprintf(path, sizeof(path), "output/bsearch_%u.out", num);
    dump_output(path, &idx, 1);

out_free:
    /* buffer cleanup */
    free(ref);
    free(buff);

    return ret;
}


int32_t
usage(void)
{
    printf("Usage: ./bin/check [{qsort|bsearch} TEST_NUM]\n");

    return -1;
}


int32_t
main(int32_t argc, char *argv[])
{
    uint32_t (*test_func)(uint32_t);
    float score = 0;
    double   total = 0;

    /* run specific test */
    if (argc == 3) {
        /* determine if qsort or bsearch test needs to be executed */
        if (!strcmp(argv[1], "qsort"))
            test_func = run_qsort_test;
        else if (!strcmp(argv[1], "bsearch"))
            test_func = run_bsearch_test;
        else
            return usage();

        /* run test and report score */
        score = (float)test_func(atoi(argv[2]));
        printf("Test %02d ................. %s: %.2fp\n",
               atoi(argv[2]), score ? "PASSED" : "FAILED", score);

        return 0;
    }
    /* run all tests */
    else if (argc == 1) {
        printf("---------------TASK 2---------------\n");

        printf("           (qsort suite)\n");
        for (uint32_t i = 1; i <= NUM_TESTS; i++) {
            score  = (float)(2 * run_qsort_test(i));
            total += score;
            printf("Test %2d ................. %s: %.2fp\n",
                   i, score ? "PASSED" : "FAILED", score);
        }

        printf("\n         (bsearch suite)\n");
        for (uint32_t i = 1; i <= NUM_TESTS; i++) {
            score  = (float)run_bsearch_test(i);
            total += score;
            printf("Test %02d ................. %s: %.2fp\n",
                   i, score ? "PASSED" : "FAILED", score);
        }

        printf("\nTASK 2 SCORE: %.2f / 30.00\n\n", total);

        return 0;
    }

    return usage();
}

