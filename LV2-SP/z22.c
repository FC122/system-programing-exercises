#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define MAX_PATH 1024
#define VELIKA_DATOTEKA 1048576 // 1 MB

void pretrazi_direktorij(const char *path, int *broj_velikih, time_t *najstariji_datum) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char filepath[MAX_PATH];

    if ((dir = opendir(path)) == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filepath, MAX_PATH, "%s/%s", path, entry->d_name);

        if (stat(filepath, &statbuf) == -1) {
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            pretrazi_direktorij(filepath, broj_velikih, najstariji_datum);
        } else {
            if (statbuf.st_size >= VELIKA_DATOTEKA) {
                (*broj_velikih)++;
            }

            if (statbuf.st_birthtimespec.tv_sec < *najstariji_datum) {
                *najstariji_datum = statbuf.st_birthtimespec.tv_sec;
            }
        }
    }

    closedir(dir);
}

int main() {
    int broj_velikih = 0;
    time_t najstariji_datum = time(NULL);

    pretrazi_direktorij(".", &broj_velikih, &najstariji_datum);

    struct tm *tm_info = localtime(&najstariji_datum);
    printf("Broj velikih datoteka: %d\n", broj_velikih);
    printf("Datum najstarije datoteke: %d-%02d-%02d\n", tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday);

    return 0;
}
