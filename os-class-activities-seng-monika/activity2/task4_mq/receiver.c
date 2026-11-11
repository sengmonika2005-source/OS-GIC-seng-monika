/* Seng Monika, e20231167: POSIX message queue receiver. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(void)
{
    mqd_t queue = mq_open(QUEUE_NAME, O_RDONLY);
    if (queue == (mqd_t)-1) {
        perror("mq_open");
        fputs("Hint: Did you run sender first?\n", stderr);
        return EXIT_FAILURE;
    }
    int result = EXIT_FAILURE;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    if (mq_getattr(queue, &attr) == -1) {
        perror("mq_getattr");
        goto cleanup;
    }
    if (attr.mq_msgsize <= 0 || attr.mq_msgsize > MAX_SIZE) {
        fputs("Queue message size exceeds the receive buffer.\n", stderr);
        goto cleanup;
    }
    ssize_t received;
    do {
        received = mq_receive(queue, buffer, MAX_SIZE, NULL);
    } while (received == -1 && errno == EINTR);
    if (received == -1) {
        perror("mq_receive");
        goto cleanup;
    }
    buffer[received] = '\0';
    printf("Receiver: message received from queue '%s'\n", QUEUE_NAME);
    printf("Receiver: message = \"%s\"\n", buffer);
    result = EXIT_SUCCESS;

cleanup:
    if (mq_close(queue) == -1) {
        perror("mq_close");
        result = EXIT_FAILURE;
    }
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        result = EXIT_FAILURE;
    } else {
        puts("Receiver: queue unlinked.");
    }
    return result;
}
