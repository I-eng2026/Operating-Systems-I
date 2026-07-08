#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // PID to signal provided as command line argument
  if (argc != 3) {
    fprintf(stderr, "usage: %s <PID> <SIGNAL>\n", argv[0]);
    exit(1);
  }

  else if (strcmp(argv[2], "SIGINT") == 0 || atol(argv[2]) == 2) {
    // Send signal SIGINT to PID from command line arguments
    kill(atol(argv[1]), SIGINT);
  } else if (strcmp(argv[2], "SIGTERM") == 0 || atol(argv[2]) == 15) {
    // Send signal SIGINT to PID from command line arguments
    kill(atol(argv[1]), SIGTERM);
  } else if (strcmp(argv[2], "SIGSTOP") == 0 || atol(argv[2]) == 19) {
    // Send signal SIGINT to PID from command line arguments
    kill(atol(argv[1]), SIGSTOP);
  } else if (strcmp(argv[2], "SIGCONT") == 0 || atol(argv[2]) == 18) {
    // Send signal SIGINT to PID from command line arguments
    kill(atol(argv[1]), SIGCONT);
  }

  else {
    fprintf(stderr, "Bad arg1: Signal not recognized!\n");
    exit(1);
  }

  return 0;
}
