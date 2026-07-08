// From: http://beej.us/guide/bgipc/output/html/multipage/mq.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(int argc, char *argv[])
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;
    char *endptr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s key\n", argv[0]);
        exit(1);
    }

    key = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Error: key must be an integer\n");
        exit(1);
    }

    // Connect to the queue
    if ((msqid = msgget(key, 0644)) == -1) { 
        perror("msgget");
        exit(1);
    }
    
    printf("Spock: ready to receive messages, captain!\n");

    // Spock never quits...
    for(;;) 
    { 
        // ... Until queue is destroyed by Kirk
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Spock %d: \"%s\"\n", getpid(), buf.mtext);
    }

    return 0;
}

