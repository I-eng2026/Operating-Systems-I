/*
 * Answers to Lab 4 Questions
 *
 * Part 2:
 * Q: When kirk ended, why did spock also end?
 * A: When kirk terminates (after CTRL-D), it destroys the message queue using
 *    msgctl(). Spock is blocked waiting to receive a message from that queue
 *    via msgrcv(). When the queue is destroyed, the msgrcv() call fails,
 *    causing spock to print an error and exit.
 *
 * Q: Try starting spock first. What "output" does it give? Why?
 * A: Starting spock first results in an error "msgget: No such file or
 *    directory". This is because spock.c tries to connect to an existing
 *    message queue but does not use the IPC_CREAT flag. Since kirk hasn't
 *    run yet to create the queue, msgget() fails.
 *
 * Part 3:
 * Q: Spock won't end this way [with CTRL-D]. Why?
 * A: CTRL-D sends an EOF (End of File) marker to standard input. Spock is not
 *    reading from standard input; it is blocked in the msgrcv() system call,
 *    waiting for a message to arrive in the queue. Since it's not waiting for
 *    user input, the EOF has no effect.
 *
 * Part 6:
 * Q: What does the Spock receive order tell you about process scheduling?
 * A: The order in which multiple spockKey processes receive messages is
 *    non-deterministic. When multiple processes are waiting for a message
 *    from the same queue, the kernel's scheduler decides which one will be
 *    woken up to receive the message. It does not guarantee a FIFO order
 *    or any predictable sequence, showing that the scheduling of waking
 *    processes is not necessarily fair or deterministic from a user-space
 *    perspective.
 */

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

