/* Seng Monika, e20231167: one-shot shared memory consumer. */
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
    int result = EXIT_FAILURE;
    int fd = shm_open(name, O_RDONLY, 0);
    if (fd == -1) {
        perror("shm_open");
        fputs("Hint: Did you run shm-producer first?\n", stderr);
        return EXIT_FAILURE;
    }
    void *mapping = MAP_FAILED;
    struct stat info;
    if (fstat(fd, &info) == -1) {
        perror("fstat");
        goto cleanup;
    }
    if (info.st_size != (off_t)size) {
        fputs("Unexpected shared memory size (expected 4096 bytes).\n", stderr);
        goto cleanup;
    }
    mapping = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        goto cleanup;
    }
    if (memchr(mapping, '\0', size) == NULL) {
        fputs("Shared message has no NUL terminator.\n", stderr);
        goto cleanup;
    }
    printf("Consumer: reading from shared memory '%s'\n", name);
    printf("Consumer: message = \"%s\"\n", (const char *)mapping);
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
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        result = EXIT_FAILURE;
    } else {
        puts("Consumer: shared memory unlinked.");
    }
    return result;
}
