/* Seng Monika, e20231167: POSIX message queue sender. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "common.h"

int main(void)
{
    struct mq_attr attr = {0};
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    mqd_t queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (queue == (mqd_t)-1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }
    int result = EXIT_FAILURE;
    const char message[] = "Hello from sender! This is message queue IPC.";
    /* An existing queue retains its original attributes. Validate them. */
    if (mq_getattr(queue, &attr) == -1) {
        perror("mq_getattr");
        goto cleanup;
    }
    if (attr.mq_msgsize != MAX_SIZE || attr.mq_maxmsg != 10) {
        fputs("Existing queue attributes do not match this activity.\n", stderr);
        goto cleanup;
    }
    int sent;
    do {
        sent = mq_send(queue, message, sizeof(message), 0);
    } while (sent == -1 && errno == EINTR);
    if (sent == -1) {
        perror("mq_send");
        goto cleanup;
    }
    printf("Sender: message sent to queue '%s'\n", QUEUE_NAME);
    printf("Sender: message = \"%s\"\n", message);
    result = EXIT_SUCCESS;

cleanup:
    if (mq_close(queue) == -1) {
        perror("mq_close");
        result = EXIT_FAILURE;
    }
    /* The receiver removes the queue after receiving the message. */
    return result;
}
