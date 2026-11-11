/* Seng Monika, e20231167: one-shot shared memory producer. */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    const char *name = "/OS-e20231167";
    const size_t size = 4096;
    const char message[] = "Hello, this is shared memory IPC!";
    int result = EXIT_FAILURE;
    /* Refuse to overwrite an unconsumed object from an earlier run. */
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        fputs("Hint: Consume an existing object before running the producer again.\n", stderr);
        return EXIT_FAILURE;
    }
    void *mapping = MAP_FAILED;
    if (ftruncate(fd, (off_t)size) == -1) {
        perror("ftruncate");
        goto cleanup;
    }
    mapping = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        goto cleanup;
    }
    memcpy(mapping, message, sizeof(message)); /* Include the terminating NUL. */
    printf("Producer: wrote to shared memory '%s'\n", name);
    printf("Producer: message = \"%s\"\n", message);
    result = EXIT_SUCCESS;

cleanup:
    if (mapping != MAP_FAILED && munmap(mapping, size) == -1) {
        perror("munmap");
        result = EXIT_FAILURE;
    }
    if (close(fd) == -1) {
        perror("close");
        result = EXIT_FAILURE;
    }
    /* Keep the name on success so a later consumer can open it. */
    if (result != EXIT_SUCCESS && shm_unlink(name) == -1)
        perror("shm_unlink");
    return result;
}
