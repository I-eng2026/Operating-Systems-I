#define _GNU_SOURCE // for Ubuntu
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  // process id fork id and parent ps
  int pid, fpid, ppid;

  // fork () makes a fork of the current process and returns fork process id to
  // the parent and returns the number 0 to the child.
  fpid = fork(); // Spawn child process.

  // Both parent and child continue executing from this point.
  // However, they both have their own fpid variable whose
  // value will be different for parent and child.

  pid = getpid();   // Get PID
  ppid = getppid(); // Get PID of parent

  // This will print twice!
  printf("fpid is is %d\n", fpid);

  sleep(5);

  // the order really doesnt matter it depends on the os scheduler. The sleep
  // statemnt are place strategically to print the parent first but they both
  // race to the cpu
  if (fpid == 0) // For child, fpid == 0
  {
    sleep(1);
    printf("\nIn child process: ");
    printf("FPID = %d, PID = %d, PPID = %d\n", fpid, pid, ppid);

  } else if (fpid > 0) // For parent, fpid == PID of child
  {
    printf("\nIn parent process: ");
    printf("FPID = %d, PID = %d, PPID = %d\n", fpid, pid, ppid);
  } else {
    printf("fork failed\n");
  }

  return (0);
}
