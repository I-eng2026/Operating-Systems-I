#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // PID to signal provided as command line argument
  if (argc != 2) {
    fprintf(stderr, "usage: %s PID\n", argv[0]);
    exit(1);
  }

  // Send signal SIGINT to PID from command line argument
  kill(atol(argv[1]), SIGINT);

  return 0;
}
