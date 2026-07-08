#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMKEY 75

int main(int argc, char *argv[])  
{
    int pid = atoi(argv[1]);
    int shmid;

    /* Connect to the shared memory location created by the writer. 
     * Loop until the segment is available. */
    while ((shmid = shmget(SHMKEY, 128, 0666)) == -1) {
        sleep(1);
    }
	int *pint = (int*) shmat(shmid, 0, 0);
	if (pint == (void *) -1) {
		perror("shmat");
		exit(1);
	}

	/* Wait until the writer sets the flag (pint[0] == -1). */
	while(pint[0] != -1)
        sleep(1); 
						
    /* Read and print the first batch of numbers. */
	for(int i = 0; i < 10; i++) 
		printf("%d\n", pint[i+1]);
	
    /* Reset the flag and signal the writer to continue. */
    pint[0] = 0;
	kill(pid, 10);

    /* Wait for the writer to set the flag again. */
    while(pint[0] != -1)
        sleep(1);

    /* Read and print the second batch of numbers. */
    for(int i = 0; i < 10; i++) 
		printf("%d\n", pint[i+1]);

    /* Display shared memory segments. */
    system("ipcs -m");

    /* Signal the writer to detach shmem and die. */
	kill(pid, 10); 
}
