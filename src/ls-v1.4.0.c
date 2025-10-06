/*
 * ls-v1.4.0.c
 * Enhanced LS command (v1.4.0)
 * Feature: Alphabetical Sort + Horizontal & Vertical Columns + Long Listing
 * Author: Akash Masih (BSDSF22M050)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_FILES 1024

// Function prototypes
void listFiles(const char *path, int longFlag, int horizFlag);
void displayVertical(char **files, int count);
void displayHorizontal(char **files, int count);
int compareStrings(const void *a, const void *b);
void printLongList(const char *path, char **files, int count);
void printPermissions(mode_t mode);

int main(int argc, char *argv[]) {
    const char *path = ".";
    int longFlag = 0;
    int horizFlag = 0;
    int opt;

    while ((opt = getopt(argc, argv, "lxv")) != -1) {
        switch (opt) {
            case 'l':
                longFlag = 1;
                break;
            case 'x':
                horizFlag = 1;
                break;
            case 'v':
                printf("Custom LS version 1.4.0 - Alphabetical Sort\n");
                return 0;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [directory]\n", argv[0]);
                return 1;
        }
    }

    if (optind < argc) path = argv[optind];

    listFiles(path, longFlag, horizFlag);
    return 0;
}

void listFiles(const char *path, int longFlag, int horizFlag) {
    DIR *dir;
    struct dirent *entry;
    char *filenames[MAX_FILES];
    int count = 0;

    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "ls-v1.4.0: cannot open directory '%s': %s\n", path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (entry->d_name[0] == '.') continue; // skip hidden files
        filenames[count] = strdup(entry->d_name);
        if (!filenames[count]) {
            perror("strdup");
            closedir(dir);
            exit(EXIT_FAILURE);
        }
        count++;
    }
    closedir(dir);

    // Sort alphabetically
    qsort(filenames, count, sizeof(char *), compareStrings);

    if (longFlag) {
        printLongList(path, filenames, count);
    } else if (horizFlag) {
        displayHorizontal(filenames, count);
    } else {
        displayVertical(filenames, count);
    }

    for (int i = 0; i < count; i++) free(filenames[i]);
}

int compareStrings(const void *a, const void *b) {
    char *strA = *(char **)a;
    char *strB = *(char **)b;
    return strcmp(strA, strB);
}

void displayVertical(char **files, int count) {
    int max_len = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > max_len) max_len = len;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int col_width = max_len + 2;
    int cols = term_width / col_width;
    if (cols == 0) cols = 1;
    int rows = (count + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = c * rows + r;
            if (idx < count) printf("%-*s", col_width, files[idx]);
        }
        printf("\n");
    }
}

void displayHorizontal(char **files, int count) {
    int max_len = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > max_len) max_len = len;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int col_width = max_len + 2;
    int current_width = 0;

    for (int i = 0; i < count; i++) {
        printf("%-*s", col_width, files[i]);
        current_width += col_width;
        if (current_width + col_width > term_width) {
            printf("\n");
            current_width = 0;
        }
    }
    if (current_width != 0) printf("\n");
}

void printLongList(const char *path, char **files, int count) {
    struct stat statbuf;
    for (int i = 0; i < count; i++) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, files[i]);
        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat");
            continue;
        }
        printPermissions(statbuf.st_mode);
        printf(" %ld", (long)statbuf.st_nlink);

        struct passwd *pw = getpwuid(statbuf.st_uid);
        struct group  *gr = getgrgid(statbuf.st_gid);
        printf(" %s %s", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

        printf(" %5lld", (long long)statbuf.st_size);

        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&statbuf.st_mtime));
        printf(" %s %s\n", timebuf, files[i]);
    }
}

void printPermissions(mode_t mode) {
    char perms[11];
    perms[0] = S_ISDIR(mode) ? 'd' : '-';
    perms[0] = S_ISLNK(mode) ? 'l' : perms[0];
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
    printf("%s", perms);
}
