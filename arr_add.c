
#define _GNU_SOURCE // for Ubuntu
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Pointers to arrays must be in global scope,
// so that all child threads can access them.
int *arr1;
int *arr2;
int *res;

int add_rows(void *args) {
  // The argument received by add_rows is void*, it must be typecast.
  int *ids = (int *)args;

  /* ########## PART IV ################################################## */
  // Each thread must calculate res = arr1 + arr2 for ids[0]
  // up to ids[1] (inclusive). Write code to do this below.
  /* ##################################################################### */
  /* PART IV - YOUR CODE HERE */
  int start = ids[0];
  int end = ids[1];

  for (int i = start; i <= end; i++) {
    res[i] = arr1[i] + arr2[i];
  }
  /* PART IV - YOUR CODE ENDS */

  free(args);
  exit(0); // Exit (terminate child)
}

int main(int argc, char *argv[]) {
  const int N_THREADS = 4; // Number of threads to create
  const int ARR_SIZE = 16; // Size of arrays being added
  // Number of rows for each thread to handle
  const int N_ROWS = ARR_SIZE / N_THREADS;
  const int STACK_SIZE = 16384;

  /* ########## PART I ################################################### */
  // Dynamically allocate arr1, arr2, and res. They must contain
  // ARR_SIZE integers each. Use calloc().
  // Fill arrays arr1 and arr2 with random integers between 0 and 10
  // You can google (or use ChatGPT) to find how to do this.
  /* ##################################################################### */
  /* PART I - YOUR CODE HERE */
  arr1 = calloc(ARR_SIZE, sizeof(int));
  arr2 = calloc(ARR_SIZE, sizeof(int));
  res = calloc(ARR_SIZE, sizeof(int));

  srand(time(NULL));

  for (int i = 0; i < ARR_SIZE; i++) {
    arr1[i] = rand() % 11;
    arr2[i] = rand() % 11;
  }

  /* PART I - YOUR CODE ENDS */

  char *stack[N_THREADS];    // Stack memory for each thread
  char *stackTop[N_THREADS]; // Pointer to stack top for each thread

  /* ########## PART II ################################################## */
  // Allocate N_THREADS stacks, and assign N_THREADS stackTop pointers.
  // N_THREADS is set to four, but your code below should work no matter
  // how what the value of N_THREADS is. Refer to previous clone() example
  // to see how to do this.
  /* ##################################################################### */
  /* PART II - YOUR CODE HERE */
  for (int i = 0; i < N_THREADS; i++) {
    stack[i] = malloc(STACK_SIZE);
    if (stack[i] == NULL) {
      perror("malloc");
      exit(1);
    }
    stackTop[i] = stack[i] + STACK_SIZE;
  }

  /* PART II - YOUR CODE ENDS */

  /* ########## PART III ################################################# */
  // Spawn N_THREADS child threads using clone(). stackTop[i], allocated
  // in part II, is the stack for the ith thread.
  // The fourth argument to clone, instead of being NULL as in previous
  // examples, will be an array of two integers. This fourth argument is
  // what gets passed to add_rows() as an argument.
  // These integers represent the lower and upper indices that the ith
  // thread will calculate. For example, if there are 4 threads and 16
  // elements in the arrays, the the first thread will calculate res[0-3],
  // the second will calculate res[4-7], the third res[8-11], and the fourth
  // res[12-15]. You must spawn the threads in such a way that this happens.
  // It might be helpful to look at the add_rows function above, first.
  /* ##################################################################### */
  /* PART III - YOUR CODE HERE */
  for (int i = 0; i < N_THREADS; i++) {
    int *thread_args = malloc(2 * sizeof(int));

    int start_idx = i * N_ROWS;
    int end_idx = start_idx + N_ROWS - 1;

    thread_args[0] = start_idx;
    thread_args[1] = end_idx;

    clone(add_rows, stackTop[i], CLONE_VM | CLONE_FILES, thread_args);
  }

  /* PART III - YOUR CODE ENDS */

  // Parent keeps going here. Put it to sleep for one second to
  // make sure all children finish first.
  sleep(1);

  // Here we print all three arrays so that we can verify the results.
  for (int i = 0; i < ARR_SIZE; i++)
    printf("%3d ", arr1[i]);
  printf("\n");
  for (int i = 0; i < ARR_SIZE; i++)
    printf("%3d ", arr2[i]);
  printf("\n");
  for (int i = 0; i < ARR_SIZE; i++)
    printf("%3d ", res[i]);
  printf("\n");

  free(arr1);
  free(arr2);
  free(res);

  return 0;
}
