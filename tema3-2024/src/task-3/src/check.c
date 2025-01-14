#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

#define NUM_TESTS 10 /* tests are numbered 1..10 */

static uint32_t ref_lengths[NUM_TESTS] = {4, 3, 81, 5, 28, 1, 3, 15, 12, 5516};

static uint32_t test_values[NUM_TESTS] = {1, 2, 4, 2, 4, 1, 2, 4, 5, 5};

static char test_dir[] = "./tests/";

static char *tests[NUM_TESTS] = {
    "1-simple_no_cycles/", "2-multiple_components/", "3-complex_no_cycles/",
    "4-simple_cycle/",     "5-complex_cycles/",      "6-single_node/",
    "7-self_loop/",        "8-sparse_graph/",        "9-dense_graph/",
    "10-large_graph/"};

static char in[] = "dfs.in";
static char ref[] = "dfs.ref";
static char out[] = "dfs.out";

/*
 * n - the number of nodes.
 * m - the number of edges.
 * s - the source node for dfs.
 */
static uint32_t n, m, src;
// The list of adjecency lists for every node.
static List *adj;
// List containing all heap data that must be freed at program end.
static List to_free;

static FILE *outfile;

extern uint32_t visited[10000];

/* Data structure returned by the expansion function */
typedef struct __attribute__((__packed__)) neighbours_t {
  uint32_t num_neighs;
  uint32_t *neighs;
} neighbours_t;

/* The expansion function to be called in `dfs.asm`. */
extern neighbours_t *expand(uint32_t node) {
  neighbours_t *ret = malloc(sizeof(*ret));
  list_insert_shallow(to_free, ret);

  List l = adj[node];
  ret->num_neighs = l->size;

  ret->neighs = malloc(l->size * sizeof(*ret->neighs));
  list_insert_shallow(to_free, ret->neighs);

  LNode curr_node = l->head;
  for (uint32_t i = l->size - 1; i < l->size;
       --i, curr_node = curr_node->next) {
    ret->neighs[i] = *(uint32_t *)curr_node->elem;
  }

  return ret;
}

/* asm implementations required */
void dfs(uint32_t node, neighbours_t *(*expand)(uint32_t node));

/* get_input - reads contents of input file into global variables.
 *  @path   : input file path
 */
void get_input(char *path) {
  FILE *fin;

  /* open input file */
  fin = fopen(path, "r");
  if (!fin) {
    fprintf(outfile, "unable to open \"%s\" (%s)\n", path, strerror(errno));
    return;
  }

  /* read header */
  fscanf(fin, "%u%u%u", &n, &m, &src);

  /* allocate vector of adjecency lists */
  adj = malloc(n * sizeof(*adj));
  if (!adj) {
    fprintf(outfile, "unable to allocate buffer (%s)\n", strerror(errno));
    goto out_close;
  }

  /* create an adjecency list for every nodes */
  for (uint32_t i = 0; i < n; i++) {
    adj[i] = list_create(sizeof(uint32_t));
  }

  /* read edges */
  uint32_t node1, node2;
  for (uint32_t i = 0; i < m; i++) {
    fscanf(fin, "%u%u", &node1, &node2);
    list_insert(adj[node1], &node2);
  }

  /* cleanup sequence */
out_close:
  fclose(fin);
}

/* get_reference - reads contents of reference file into a buffer
 *  @path : reference file path
 *  @len  : number of elements in the reference file (hardcoded in ref_lengths)
 *
 *  @return : ptr to array filled with reference values
 *
 * NOTE: returned buffer must be freed by caller
 */
uint32_t *get_reference(char *path, uint32_t len) {
  FILE *fin;
  uint32_t *buff = NULL;

  /* open input file */
  fin = fopen(path, "r");
  if (!fin) {
    fprintf(outfile, "unable to open \"%s\" (%s)\n", path, strerror(errno));
    return NULL;
  }

  /* allocate buffer for all elements */
  buff = malloc(len * sizeof(int32_t));
  if (!buff) {
    fprintf(outfile, "unable to allocate buffer (%s)\n", strerror(errno));
    goto out_close;
  }

  /* read a pre-determined number of elements */
  for (size_t i = 0; i < len; i++) fscanf(fin, "%u", &buff[i]);

out_close:
  fclose(fin);

  return buff;
}

/* run_test - executes a specific test
 *  @num : test number (starts from 1)
 *
 *  @return : 1 if successful; 0 otherwise
 */
uint32_t run_test(uint32_t num) {
  num--;
  char path[100]; /* file path        */
  uint32_t out_buff;
  uint32_t ref_len = ref_lengths[num];
  uint32_t *ref_buff; /* reference array  */
  uint32_t ret = 0;   /* return value     */

  /* read input file */
  snprintf(path, sizeof(path), "%s%s%s", test_dir, tests[num], in);
  get_input(path);

  /* read reference file */
  snprintf(path, sizeof(path), "%s%s%s", test_dir, tests[num], ref);
  ref_buff = get_reference(path, ref_len);
  if (!ref_buff) return -1;

  /* redirect stdout to output file */
  snprintf(path, sizeof(path), "%s%s%s", test_dir, tests[num], out);
  FILE *out = freopen(path, "w+", stdout);
  if (!out) {
    fprintf(outfile, "unable to open \"%s\" (%s)", path, strerror(errno));
    return -1;
  }

  /* run dfs */
  dfs(src, expand);
  fflush(stdout);
  memset(visited, 0, sizeof(visited));

  /* prepare to read the output */
  rewind(stdout);

  /* compare output with reference */
  for (size_t i = 0; i < ref_len; i++) {
    if (fscanf(stdout, "%u\n", &out_buff) == 0) goto out_free;
    if (out_buff != ref_buff[i]) goto out_free;
  }

  /* success */
  ret = 1;

out_free:
  /* test cleanup */
  free(ref_buff);
  list_clear(to_free);
  for (uint32_t i = 0; i < n; i++) list_free(adj[i]);
  free(adj);

  return ret == 0 ? 0 : test_values[num];
}

int32_t usage(void) {
  fprintf(outfile, "Usage: ./bin/check [TEST_NUM]\n");

  return -1;
}

int32_t invalid_test(void) {
  fprintf(outfile, "Test number out of range\n");

  return -1;
}

int32_t main(int32_t argc, char *argv[]) {
  float score = 0;
  double total = 0;

  to_free = list_create(sizeof(void *));

  /* save stdout */
  int saved_stdout = dup(fileno(stdout));
  if (saved_stdout < 0) {
    fprintf(outfile, "unable to duplicate stdout (%s)", strerror(errno));
    return -1;
  }

  outfile = fdopen(saved_stdout, "w");

  /* run specific test */
  if (argc == 2) {
    /* determine if test number is valid */
    uint32_t test_num = atoi(argv[1]);
    if (test_num < 1 || 10 < test_num) {
      free(to_free);
      return invalid_test();
    }

    /* run test and report score */
    score = (float)run_test(test_num);
    fprintf(outfile, "Test %2d ................. %s: %.2fp\n", test_num,
            score ? "PASSED" : "FAILED", score);

    free(to_free);
    return 0;
  }
  /* run all tests */
  else if (argc == 1) {
    fprintf(outfile, "---------------TASK 3---------------\n");

    for (uint32_t i = 1; i <= NUM_TESTS; i++) {
      score = (float)run_test(i);
      total += score;
      fprintf(outfile, "Test %02d ................. %s: %.2fp\n", i,
              score ? "PASSED" : "FAILED", score);
    }

    fprintf(outfile, "\nTASK 3 SCORE: %.2f / 30.00\n\n", total);

    free(to_free);
    return 0;
  }

  free(to_free);
  return usage();
}
