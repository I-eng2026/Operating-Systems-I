/*
In this program we use the clone() function to spawn a
child process in two different ways:

A child spawned as a HEAVYWEIGHT process gets a copy of
parents memory, file descriptors, etc, but any changes in
child would be only for the child.

A child spawned as a LIGHTWEIGHT process (thread) shares
memory and files with its parent by using flags CLONE_VM
and CLONE_FILES.

The difference is only in a single line, seen below.
Comment out the appropriate line depending on which type
of child you wish to spawn
*/

#define _GNU_SOURCE // for Ubuntu
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Globals in scope for both main() and do_something()
// this is bad practice which is why you don't see this often in C.
// You cannot call printf or any other function you can only declare global
// variables
int var = 0;

int do_something(void *arg) {
  printf("\nChild in do_something():\n");
  printf("  My PID is: %d\n", getpid());
  printf("  My parent's PID is %d\n\n", getppid());
  var = 42; // Change global variable
  exit(0);  // Exit (terminate child)
}

int main(int argc, char *argv[]) {
  char *stack, *stackTop; // Pointers for child stack
  /* set stack size to 64KB 64 * 2^10  */
  const int STACK_SIZE = 65536; // Note stack grows down
  stack = malloc(STACK_SIZE);   // Allocate stack for child
  if (stack == NULL) {
    perror("malloc");
    exit(1);
  }
  stackTop = stack + STACK_SIZE; // Note stack grows down

  var = 9;
  printf("VAR in parent before clone: %d\n", var);

  // Calling clone() this way spawns a HEAVYWEIGHT process
  // returns the process id of the clone. Takes in the function that will live
  // in the allocated memory. Also takes in the stackTop because that is where
  // the data from our function are going to start pushing. Takes in a 0 for a
  // heavyweight process or a pipe separated list of constant meant to describe
  // the different entities we want to share. And the last argument is the data
  // to be passed to the function (do_something in our case). It can Be a list
  // or it can be NULL
  // int cret = clone(do_something, stackTop, 0, NULL);
  // Calling clone() this way spawns a LIGHTWEIGHT process as we are sharing the
  // virtual memory and the file descriptors
  int cret = clone(do_something, stackTop, CLONE_VM | CLONE_FILES, NULL);

  if (cret == -1) {
    perror("clone() failed");
    exit(0);
  }

  // Parent keeps going here, child continues execution in do_something()
  printf("Parent PID after clone() %d\n", getpid());
  sleep(1);

  printf("VAR in parent after clone/sleep: %d\n", var);

  return 0;
}
