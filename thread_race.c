#define _DEFAULT_SOURCE // To stop replit warning about usleep()
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

/*
This program spawns a ULT using the pthreads library.
Both the main thread and the child thread increment a
shared global variable, myglobal, 20 times each.
Compile as folllows:
gcc -pthread -o threadRace threadRace.c
*/

int myglobal = 0;

// Function executed by child ULT
void *thread_function(void *arg) 
{
    // This for loop increments myglobal each iteration
    // in a very roundabout way. This is to increase
    // the likelihood of the increments interleaving
    // with those in main thread.
    for (int i = 0; i < 20; i++) 
    {
        int j = myglobal;
        j = j + 1;
        printf(".");    // Child thread prints dots
        fflush(stdout);
        usleep(200);    // Sleep 200 us
        myglobal = j;   // Save j back in myglobal
    }
	
    return NULL;
}

int main(void) 
{
    pthread_t mythread;
	
    // Create child ULT, exit if it fails.
    if (pthread_create(&mythread, NULL, thread_function, NULL))
    {
        printf("Error creating thread.\n");
        exit(1);
    }

    // Increment shared variable myglobal 20 times
    for (int i = 0; i < 20; i++) 
	{
		myglobal = myglobal + 1;
		printf("o");    // Main thread prints o
		fflush(stdout);
		usleep(210);    // Sleep 210 us
	}

    // Wait for child thread to complete, exit if it fails
	if (pthread_join(mythread, NULL)) {
		printf("error joining thread.");
		exit(1);
	}

	printf("\nmyglobal equals %d\n", myglobal);

	exit(0);
}
 

