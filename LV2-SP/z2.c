#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <limits.h>


#define SIZE_THRESHOLD 1024 // 1 kB

void traverse_directory(const char *dir_name, int *num_big_files, time_t *oldest_file_time) {
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", dir_name, entry->d_name);
        struct stat st;
        if (lstat(path, &st) == -1) {
            perror("lstat");
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;  // skip "." and ".." directories
            }
            traverse_directory(path, num_big_files, oldest_file_time);
        } else if (S_ISREG(st.st_mode)) {
            if (st.st_size > SIZE_THRESHOLD) {
                (*num_big_files)++;
            }
            if (st.st_mtime > *oldest_file_time) {
                *oldest_file_time = st.st_mtime;
            }
        }
    }

    closedir(dir);
}

int main() {
    int num_big_files = 0;
    time_t oldest_file_time = time(NULL);

    traverse_directory(".", &num_big_files, &oldest_file_time);

    printf("Number of big files: %d\n", num_big_files);

    struct tm *timeinfo = localtime(&oldest_file_time);
    char time_str[80];
    strftime(time_str, 80, "%d.%m.%Y", timeinfo);
    printf("Oldest file date: %s\n", time_str);

    return 0;
}
