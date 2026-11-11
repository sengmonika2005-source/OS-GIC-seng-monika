
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        const char *error = "Error opening directory\n";
        write(2, error, strlen(error));
        return 1;
    }

    struct dirent *entry;
    struct stat fileStat;
    char buffer[512];

    snprintf(buffer, sizeof(buffer),
             "%-30s %10s\n", "Filename", "Size (bytes)");
    write(1, buffer, strlen(buffer));

    snprintf(buffer, sizeof(buffer),
             "%-30s %10s\n", "------------------------------", "----------");
    write(1, buffer, strlen(buffer));

    while ((entry = readdir(dir)) != NULL) {
        if (stat(entry->d_name, &fileStat) == 0) {
            snprintf(buffer, sizeof(buffer),
                     "%-30s %10ld\n",
                     entry->d_name, (long)fileStat.st_size);

            write(1, buffer, strlen(buffer));
        }
    }

    closedir(dir);
    return 0;
}
