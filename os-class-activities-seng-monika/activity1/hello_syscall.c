#include <unistd.h>
#include <string.h>

int main() {
    const char *message = "Hello, System Call!\n";
    write(STDOUT_FILENO, message, strlen(message));
    return 0;
}
