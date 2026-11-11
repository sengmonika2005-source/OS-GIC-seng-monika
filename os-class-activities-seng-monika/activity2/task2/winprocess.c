/* Seng Monika, e20231167: native Windows process creation. */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static int close_child_handles(PROCESS_INFORMATION *child)
{
    int result = EXIT_SUCCESS;
    if (!CloseHandle(child->hThread)) {
        fprintf(stderr, "CloseHandle(thread) failed (error %lu).\n", GetLastError());
        result = EXIT_FAILURE;
    }
    if (!CloseHandle(child->hProcess)) {
        fprintf(stderr, "CloseHandle(process) failed (error %lu).\n", GetLastError());
        result = EXIT_FAILURE;
    }
    return result;
}

int main(int argc, char *argv[])
{
    STARTUPINFOA startup = {0};
    PROCESS_INFORMATION child = {0};
    startup.cb = sizeof(startup);
    char application[MAX_PATH];
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [full-path-to-mspaint.exe]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        int written = snprintf(application, sizeof(application), "%s", argv[1]);
        if (written < 0 || written >= (int)sizeof(application)) {
            fputs("Paint executable path is too long.\n", stderr);
            return EXIT_FAILURE;
        }
    } else {
        UINT length = GetSystemDirectoryA(application, MAX_PATH);
        if (length == 0 || length >= MAX_PATH) {
            fprintf(stderr, "GetSystemDirectory failed or path is too long (error %lu).\n",
                    GetLastError());
            return EXIT_FAILURE;
        }
        int written = snprintf(application + length, MAX_PATH - length, "\\mspaint.exe");
        if (written < 0 || (UINT)written >= MAX_PATH - length) {
            fputs("Paint executable path is too long.\n", stderr);
            return EXIT_FAILURE;
        }
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Parent process (PID: %lu) - creating child...\n", GetCurrentProcessId());
    printf("Application: %s\n", application);
    if (!CreateProcessA(application, NULL, NULL, NULL, FALSE, 0,
                        NULL, NULL, &startup, &child)) {
        fprintf(stderr, "CreateProcess failed (error %lu).\n", GetLastError());
        return EXIT_FAILURE;
    }
    puts("Child process created!");
    printf("  Child PID:       %lu\n", child.dwProcessId);
    printf("  Child Thread ID: %lu\n", child.dwThreadId);
    puts("Parent: waiting for child (mspaint) to exit...");
    puts(">>> Observe the processes, then close the launched Paint window. <<<");
    DWORD wait_result = WaitForSingleObject(child.hProcess, INFINITE);
    if (wait_result != WAIT_OBJECT_0) {
        fprintf(stderr, "WaitForSingleObject failed (result %lu, error %lu).\n",
                wait_result, GetLastError());
        close_child_handles(&child);
        return EXIT_FAILURE;
    }
    DWORD exit_code;
    if (!GetExitCodeProcess(child.hProcess, &exit_code)) {
        fprintf(stderr, "GetExitCodeProcess failed (error %lu).\n", GetLastError());
        close_child_handles(&child);
        return EXIT_FAILURE;
    }
    printf("Parent: child has exited with status %lu. Cleaning up.\n", exit_code);
    int result = close_child_handles(&child);
    return exit_code == 0 ? result : EXIT_FAILURE;
}
