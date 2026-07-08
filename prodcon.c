#define _DEFAULT_SOURCE // To stop replit warning about usleep()
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int terminate = 0; // Termination flag
int buf[32];       // Shared buffer
int xcount = 0;
int ycount = 0;

sem_t semEmpty;
sem_t semFull;
sem_t semAccess;

// Function to be spawned as a producer thread
void *producer(void *arg) {
  /*
        TODO:
        - Produce items into buf[] until terminate is 1
        - Refer to lab description for full requirements.
  */
  while (terminate != 1) {
    int x = (rand() % 20) + 1;
    sem_wait(&semEmpty);
    sem_wait(&semAccess);
    printf("%d produced.\n", x);
    buf[xcount] = x;
    if (xcount < (sizeof(buf) / sizeof(buf[0])) - 1) {
      xcount++;
    } else {
      xcount = 0;
    }
    sem_post(&semAccess);
    sem_post(&semFull);
    usleep(50000);
  }
  return NULL;
}

// Function to be spawned as a consumer thread
void *consumer(void *arg) {
  /*
        TODO:
        - Consume items from buf[] until terminate is 1
        - Refer to lab description for full requirements.
  */
  while (terminate != 1) {
    int y;
    sem_wait(&semFull);
    sem_wait(&semAccess);
    y = buf[ycount];
    printf("%d consumed.\n", y);
    if (ycount < (sizeof(buf) / sizeof(buf[0])) - 1) {
      ycount++;
    } else {
      ycount = 0;
    }
    sem_post(&semAccess);
    sem_post(&semEmpty);
    usleep(50000);
  }

  return NULL;
}

void intHandler(int sigNum) {
  if (sigNum == SIGINT)
    terminate = 1;
}

int main(void) {
  srand(time(NULL));
  pthread_t prod, con;
  __sighandler_t ret;
  ret = signal(SIGINT, intHandler);

  /*
        TODO:
        - Create and register signal handler for SIGINT
        - create() both threads
        - join() both threads
        - Refer to lab description for full requirements.
  */

  sem_init(&semEmpty, 0, (sizeof(buf) / sizeof(buf[0])));
  sem_init(&semFull, 0, 0);
  sem_init(&semAccess, 0, 1);

  if (pthread_create(&prod, NULL, producer, NULL)) {
    printf("Error creating thread.\n");
    exit(1);
  }

  if (pthread_create(&con, NULL, consumer, NULL)) {
    printf("Error creating thread.\n");
    exit(1);
  }

  if (pthread_join(prod, NULL)) {
    printf("error joining thread.\n");
    exit(1);
  }

  if (pthread_join(con, NULL)) {
    printf("error joining thread.\n");
    exit(1);
  }
  sem_destroy(&semEmpty);
  sem_destroy(&semFull);
  sem_destroy(&semAccess);

  printf("Producer and consumer terminated!\n");
  printf("Exiting...\n");
  exit(0);
}
