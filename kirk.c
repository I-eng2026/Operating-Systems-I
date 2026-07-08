// From: http://beej.us/guide/bgipc/output/html/multipage/mq.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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

    // Create a key from the filename "kirk.c" and the char 'B'
    // Same filename, same character, same key. Spock will create 
    // The same key, ensuring both connect to the same queue.
    if ((key = ftok("kirk.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Create message queue using key, set appropriate flags.
    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Enter lines of text, ^D to quit:\n");
    buf.mtype = 1; // We don't really care in this case

    // Loop to send messages to queue until CTRL-D is typed
    while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) 
	{
        int len = strlen(buf.mtext);

        // Ditch newline at end, if it exists
        if (buf.mtext[len-1] == '\n') 
			buf.mtext[len-1] = '\0';

        if (msgsnd(msqid, &buf, len+1, 0) == -1) // len+1 for '\0'
            perror("msgsnd");
    }

    // Remove message queue
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}

