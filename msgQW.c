#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MSG_SIZE 100
#define QUEUE_NAME "/myqueue"
#define MAX_MESSAGES 10

int main() {
    mqd_t mq;
    char msg[MAX_MSG_SIZE];
    struct mq_attr attr;

    // Set attributes for the message queue
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Open the message queue (create if doesn't exist)
    mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Send 10 messages to the queue
    for (int i = 0; i < 10; i++) {
        printf("Enter message %d: ", i + 1);
        fgets(msg, MAX_MSG_SIZE, stdin);
        msg[strcspn(msg, "\n")] = '\0'; 

        // Send message to the queue
        if (mq_send(mq, msg, strlen(msg) + 1, 0) == -1) {
            perror("mq_send");
            exit(1);
        }
        printf("Message %d sent\n", i + 1);
        sleep(1); 
    }

    
    mq_close(mq);
    return 0;
}
