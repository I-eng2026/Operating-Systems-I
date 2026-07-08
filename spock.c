// From: http://beej.us/guide/bgipc/output/html/multipage/mq.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    // Same key as kirk.c 
    if ((key = ftok("kirk.c", 'B')) == -1) {  
        perror("ftok");
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
        printf("Spock: \"%s\"\n", buf.mtext);
    }

    return 0;
}

