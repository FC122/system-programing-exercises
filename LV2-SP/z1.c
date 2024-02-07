#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    int num_dirs = 0, num_files = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".DS_Store") == 0) {
            continue;
        }
        struct stat st;
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", argv[1], entry->d_name);
        if (lstat(path, &st) == -1) {
            perror("lstat");
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            num_dirs++;
             printf(" %s\n", entry->d_name);
        } else {
             printf(" %s\n", entry->d_name);
            num_files++;
        }
    }

    closedir(dir);

    printf("Number of directories: %d\n", num_dirs);
    printf("Number of files: %d\n", num_files);

    return 0;
}
