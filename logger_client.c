#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include "common.h"

int main() {
    mqd_t mq;
    LogMessage msg;

    // Open the existing queue
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("Client: mq_open failed. Is the server running?");
        exit(1);
    }

    msg.Client_pid = getpid();
    strcpy(msg.log_level, "INFO");
    
    // Simulate sending multiple workloads
    for (int i = 1; i <= 3; i++) {
        snprintf(msg.message, MAX_MSG_SIZE, "Test log message line #%d", i);
        
        printf("[Client %d] Sending message %d...\n", getpid(), i);
        if (mq_send(mq, (const char *)&msg, sizeof(LogMessage), 0) == -1) {
            perror("Client: mq_send failed");
        }
        sleep(1); // Simulate time between events
    }

    mq_close(mq);
    printf("[Client %d] Finished and disconnected.\n", getpid());
    return 0;
}
