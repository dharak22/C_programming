#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>  // For O_NONBLOCK

#define MAX_MSG_SIZE 100
#define QUEUE_NAME "/myqueue"

int main() {
    mqd_t mq;
    char msg[MAX_MSG_SIZE];
    struct mq_attr attr;

    
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_NONBLOCK); // write | O_NONBLOCK
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Polling for messages...\n");
    // if you do not write O_NONBLOCK it stays here only it is a blocking mechanism
    // if you do write O_NONBLOCK it gets inside the loop and continuously polls 
    while (1) {
        ssize_t msg_size = mq_receive(mq, msg, MAX_MSG_SIZE, NULL);

        if (msg_size == -1) {
            if (errno == EAGAIN) {
                printf("No messages available, polling again...\n");
                sleep(1);  
            } else {
                perror("mq_receive");
                break;
            }
        } else {
            printf("Received message: %s\n", msg);
        }
    }

    // Close the message queue
    mq_close(mq);
    return 0;
}
