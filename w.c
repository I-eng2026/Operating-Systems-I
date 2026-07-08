#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHMKEY 75
int shmid;

void handler(int);
void cleanup(int);

int main (void) 
{
    /* Force any unexpected signal to call handler function cleanup() */
    for (int i = 1; i <= 20; i++)  
        signal(i, cleanup);  

    signal(10, handler);

    /* Allocate the shared memory. */
    shmid = shmget(SHMKEY, 128, 0666|IPC_CREAT);
    int *pint = (int*) shmat(shmid, 0, 0);
	
    /* Fills array with 0-9 then sets flag to alert reader. */
    for(int i = 0; i < 10; i++)  
        pint[i+1] = i;
    pint[0] = -1;

    /* The writer will block here until a signal is received from the reader. */
    pause();

    /* Fills array with 10-19 then resets flag to alert reader again. */
    for(int i = 0; i < 10; i++)  
        pint[i+1] = i+10;
    pint[0] = -1;

    /* The writer will block here until a signal is received from the reader. */
    pause();
	
    /* Detach and free the shared memory region. */
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}

// This function is called when the writer receives a signal.
// It does nothing, its purpose is to interrupt the pause().
void handler(int x) 
{ 
    // nothing
}

// This function is called when the writer receives a signal.
// It detaches and frees the shared memory region.
void cleanup(int x) 
{ 
	shmctl(shmid, IPC_RMID, 0);  
	exit(0);
}
