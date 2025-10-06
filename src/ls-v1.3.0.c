/*
 * ls-v1.3.0.c
 * Enhanced LS command (v1.3.0)
 * Features: 
 *  - Default down-then-across column display
 *  - Long listing format (-l)
 *  - Horizontal row-major column display (-x)
 * Author: Akash Masih (BSDSF22M050)
 */

#define _GNU_SOURCE  // ensures optind is defined

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

#define MAX_FILES 1024
#define MAX_NAME 256

typedef enum { MODE_DEFAULT, MODE_LONG, MODE_HORIZONTAL } DisplayMode;

// Function prototypes
void listFiles(const char *path, DisplayMode mode);
void displayVerticalColumns(char files[][MAX_NAME], int count);
void displayHorizontalColumns(char files[][MAX_NAME], int count);
int compareStrings(const void *a, const void *b);
void printPermissions(mode_t mode);

int main(int argc, char *argv[]) {
    const char *path = ".";
    DisplayMode mode = MODE_DEFAULT;

    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, "lxv")) != -1) {
        switch (opt) {
            case 'l':
                mode = MODE_LONG;
                break;
            case 'x':
                mode = MODE_HORIZONTAL;
                break;
            case 'v':
                printf("Custom LS version 1.3.0 - Horizontal Column Display\n");
                return 0;
            default:
                fprintf(stderr, "Usage: %s [-l|-x] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Directory argument
    if (optind < argc) path = argv[optind];

    listFiles(path, mode);
    return 0;
}

// List files from directory
void listFiles(const char *path, DisplayMode mode) {
    DIR *dir;
    struct dirent *entry;
    char files[MAX_FILES][MAX_NAME];
    int count = 0;

    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "ls-v1.3.0: cannot open directory '%s': %s\n", path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            strncpy(files[count], entry->d_name, MAX_NAME - 1);
            files[count][MAX_NAME - 1] = '\0';
            count++;
        }
    }
    closedir(dir);

    // Sort filenames alphabetically
    qsort(files, count, MAX_NAME, compareStrings);

    // Call the correct display function
    switch (mode) {
        case MODE_LONG:
            for (int i = 0; i < count; i++) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, files[i]);
                struct stat statbuf;
                if (lstat(fullpath, &statbuf) == -1) {
                    perror("lstat");
                    continue;
                }

                printPermissions(statbuf.st_mode);
                printf(" %2lu ", statbuf.st_nlink);

                struct passwd *pwd = getpwuid(statbuf.st_uid);
                struct group *grp = getgrgid(statbuf.st_gid);
                printf("%s %s ", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");

                printf("%5ld ", statbuf.st_size);

                char timebuf[80];
                struct tm *tm_info = localtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
                printf("%s ", timebuf);

                printf("%s\n", files[i]);
            }
            break;
        case MODE_HORIZONTAL:
            displayHorizontalColumns(files, count);
            break;
        default:
            displayVerticalColumns(files, count);
    }
}

// Compare strings for sorting
int compareStrings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

// Print permissions for long listing
void printPermissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

// Display default vertical "down then across" columns
void displayVerticalColumns(char files[][MAX_NAME], int count) {
    int max_len = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > max_len) max_len = len;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int col_width = max_len + 2;

    int num_cols = term_width / col_width;
    if (num_cols == 0) num_cols = 1;
    int num_rows = (count + num_cols - 1) / num_cols;

    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            int idx = col * num_rows + row;
            if (idx < count) printf("%-*s", col_width, files[idx]);
        }
        printf("\n");
    }
}

// Display horizontal row-major columns (-x)
void displayHorizontalColumns(char files[][MAX_NAME], int count) {
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
