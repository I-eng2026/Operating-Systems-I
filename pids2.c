#define _GNU_SOURCE  //for Ubuntu
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (void) 
{
   int pid, fpid, ppid;
   
   fpid = fork ();
   
   printf ("fpid is is %d\n", fpid);
   sleep(5);
   
   if (fpid > 0) 
   {
       pid  = getpid();
       ppid = getppid();
       printf("\nIn parent process: ");
       printf("FPID = %d, PID = %d, PPID = %d\n", fpid, pid, ppid);
   }
   else if (fpid == 0) 
   {
       sleep(1);
       pid  = getpid();
       ppid = getppid();
       printf("\nIn child process: ");
       printf("FPID = %d, PID = %d, PPID = %d\n", fpid, pid, ppid);
   }
   else 
   {
       printf ("fork failed\n");
   }
   
   return (0);
}
