/* Seng Monika, e20231167: Linux process creation. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    /* Line buffering also makes redirected observation output immediately visible. */
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0) {
        fputs("Cannot configure stdout buffering.\n", stderr);
        return EXIT_FAILURE;
    }
    printf("Parent process (PID: %ld) - creating child...\n", (long)getpid());
    if (fflush(stdout) == EOF) {
        perror("fflush");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        printf("Child process (PID: %ld, Parent PID: %ld)\n",
               (long)getpid(), (long)getppid());
        puts("Child: sleeping 30s - observe the process tree now!");
        unsigned int remaining = 30;
        while (remaining != 0)
            remaining = sleep(remaining);
        puts("Child: now executing 'ls -la'...");
        if (fflush(stdout) == EOF) {
            perror("fflush");
            _exit(EXIT_FAILURE);
        }
        execlp("ls", "ls", "-la", (char *)NULL);
        perror("execlp");  /* A successful exec never returns here. */
        _exit(EXIT_FAILURE);
    }

    int status;
    printf("Parent: waiting for child (PID: %ld) to finish...\n", (long)pid);
    pid_t waited;
    do {
        waited = waitpid(pid, &status, 0);
    } while (waited == -1 && errno == EINTR);
    if (waited == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("Parent: child exited with status %d\n", code);
        puts("Parent: done.");
        return code;
    }
    if (WIFSIGNALED(status))
        fprintf(stderr, "Parent: child terminated by signal %d\n", WTERMSIG(status));
    return EXIT_FAILURE;
}
