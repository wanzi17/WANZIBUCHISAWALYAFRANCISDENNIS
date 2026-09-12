#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>
#include "common.h"

mqd_t mq;

void cleanup_and_exit(int signo) {
    printf("\n[Server] Shutting down. Cleaning up IPC resources...\n");
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    exit(0);
}

int main() {
    struct mq_attr attr;
    LogMessage msg;

    // Handle Ctrl+C (SIGINT) for graceful cleanup
    signal(SIGINT, cleanup_and_exit);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(LogMessage);
    attr.mq_curmsgs = 0;

    // Create the message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Server: mq_open failed");
        exit(1);
    }

    printf("[Server] Logger started (PID: %d). Awaiting messages...\n", getpid());

    while (1) {
        // Read from queue (blocks automatically if queue is empty)
        if (mq_receive(mq, (char *)&msg, sizeof(LogMessage), NULL) == -1) {
            perror("Server: mq_receive failed");
            continue;
        }

        // Process the workload
        printf("[LOG][%s] (Client PID %d): %s\n", msg.log_level, msg.Client_pid, msg.message);
    }

    return 0;
}
