#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>

#define SHMKEY 75

int main(int argc, char *argv[])  
{
    int pid = atoi(argv[1]);

    /* Here, the reader is connecting to the same shared memory location 
    that the writer created. We know it's the same, because both the 
    reader and the writer use the same SHMKEY = 75 */
	int shmid = shmget(SHMKEY, 128, 0666);
	int *pint = (int*) shmat(shmid, 0, 0);

	/* Here, the reader sleeps for 1 second repeatedly until pint[0] is -1. 
    The reader won't move past this point until the writer has finished 
    writing, because the writer sets the first value to -1 last. */
	while(pint[0] != -1)
        sleep(1); 
						
    /* When the writer is finished writing, the reader moves past the
    busy waiting loop and reads from the shared memory. it will print
    the first 10 values from the shared memory region */    
	for(int i = 0; i < 10; i++) 
		printf("%d\n", pint[i]);    // print shared memory contents 
	
    /* This sends the signal 10 to the writer, which lets the writer move
    on from the pause() statement and execute the cleanup() function */
	kill(pid, 10); 
}
