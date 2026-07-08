#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHMKEY 75
int shmid;

void cleanup(int);

int main (void) 
{
    /* Force any signal from 1-20 (remember lab #1!) sent to this 
    process to call handler function cleanup(), which will detach 
    shared mem before exiting. */
    for (int i = 1; i <= 20; i++)  
        signal(i, cleanup);  

    /* Allocate the shared memory. "pint" is a pointer to the start 
    of the shared memory region. This pointer in the reader will point 
    to the same memory region */
    shmid = shmget(SHMKEY, 128, 0666|IPC_CREAT); // 0666 == octal 666
    int *pint = (int*) shmat(shmid, 0, 0);
	
    /* This for loop writes the values 0-9 into the shared memory 
    region. When this is done, the first value is set to -1 */
    for(int i = 0; i < 10; i++)  
        pint[i] = i;    // load sh. mem with ints 0-10 
    pint[0] = -1;       // load -1 into start of shmem 

    /* The writer will block here (pause()) until a signal is received. 
    The signal will be sent by the reader, using the kill() function. */
    pause(); 
	
	return 0;
}

// This function is called when the writer receives a signal.
// It detaches and frees the shared memory region.
void cleanup(int x) 
{ 
	shmctl(shmid, IPC_RMID, 0);  
	exit(0);
}
