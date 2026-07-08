#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// This is the handler function that will be called
// when P2 receives the SIGINT signal.
void intHandler(int sigNum) {
  // If we did in fact receive SIGINT, print message.
  // Somewhat redundant, since this handler will be
  // registered with SIGINT (see below)
  if (sigNum == SIGINT)
    printf("\nReceived SIGINT, ignoring…\n");
}

void termHandler(int sigNum) {
  if (sigNum == SIGTERM)
    printf("\nReceived SIGTERM, ignoring…\n");
}

int main(void) {
  __sighandler_t ret;
  ret = signal(SIGINT, intHandler);
  ret = signal(SIGTERM, termHandler);

  // If the handler fails to register, ret will == SIG_ERR
  if (ret == SIG_ERR) {
    fprintf(stderr, "Oops! Handler failed to register\n");
    exit(1);
  }

  // No command line argument for this one. It simply
  // prints "P2" once per second for one minute.
  for (int i = 1; i <= 60; i++) {
    sleep(1);
    fprintf(stdout, "%s", "P2");
    fflush(stdout);
  }
}
