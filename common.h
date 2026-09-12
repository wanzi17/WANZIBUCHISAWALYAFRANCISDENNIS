#ifndef COMMON_H
#define COMMON_H

#define QUEUE_NAME  "/log_system_queue"
#define MAX_MSG_SIZE 256

// Structured log format
typedef struct {
    int Client_pid;          // Fixed the typo here
    char log_level[10];      // Fixed: changed from char to a string array to hold "INFO"
    char message[MAX_MSG_SIZE];
} LogMessage;

#endif
