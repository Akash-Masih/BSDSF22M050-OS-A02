/*
 * ls-v1.5.0.c
 * Enhanced LS command (v1.5.0)
 * Feature: Colorized Output Based on File Type
 * Author: Akash Masih (BSDSF22M050)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>

/* Constants */
#define MAX_FILES 1024
#define MAX_NAME 256

/* ANSI color codes */
#define COLOR_RESET   "\033[0m"
#define COLOR_BLUE    "\033[0;34m"   // directories
#define COLOR_GREEN   "\033[0;32m"   // executables
#define COLOR_RED     "\033[0;31m"   // archives (.tar, .gz, .zip)
#define COLOR_PINK    "\033[0;35m"   // symlinks
#define COLOR_REVERSE "\033[7m"      // special files

/* Function prototypes */
void listFiles(const char *path, bool longList, bool horizontal);
int compareStrings(const void *a, const void *b);
void displayInColumns(char **files, int count, const char *path, bool colorize);
void displayLongList(char **files, int count, const char *path, bool colorize);
void printFileWithColor(const char *path, const char *filename);

/* Main */
int main(int argc, char *argv[]) {
    const char *path = ".";
    bool longList = false;
    bool horizontal = false;
    (void)horizontal; // suppress unused warning

    int opt;
    while ((opt = getopt(argc, argv, "lxv")) != -1) {
        switch (opt) {
            case 'l':
                longList = true;
                break;
            case 'x':
                horizontal = true;
                break;
            case 'v':
                printf("Custom LS version 1.5.0 - Colorized Output\n");
                return 0;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [directory]\n", argv[0]);
                return 1;
        }
    }

    if (optind < argc)
        path = argv[optind];

    listFiles(path, longList, horizontal);
    return 0;
}

/* List files in directory */
void listFiles(const char *path, bool longList, bool horizontal) {
    (void)horizontal; // suppress unused warning
    DIR *dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "ls-v1.5.0: cannot open directory '%s': %s\n", path, strerror(errno));
        return;
    }

    struct dirent *entry;
    char *filenames[MAX_FILES];
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            filenames[count] = strdup(entry->d_name);
            if (!filenames[count]) {
                fprintf(stderr, "Memory allocation failed\n");
                closedir(dir);
                return;
            }
            count++;
        }
    }
    closedir(dir);

    /* Sort alphabetically */
    qsort(filenames, count, sizeof(char *), compareStrings);

    /* Display files */
    if (longList) {
        displayLongList(filenames, count, path, true);
    } else {
        displayInColumns(filenames, count, path, true);
    }

    /* Free memory */
    for (int i = 0; i < count; i++)
        free(filenames[i]);
}

/* Compare strings for qsort */
int compareStrings(const void *a, const void *b) {
    char *const *strA = (char *const *)a;
    char *const *strB = (char *const *)b;
    return strcmp(*strA, *strB);
}

/* Display in horizontal columns */
void displayInColumns(char **files, int count, const char *path, bool colorize) {
    (void)colorize; // suppress unused warning

    int max_len = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > max_len)
            max_len = len;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int col_width = max_len + 2;

    int current_width = 0;
    for (int i = 0; i < count; i++) {
        printFileWithColor(path, files[i]);
        current_width += col_width;
        if (current_width + col_width > term_width) {
            printf("\n");
            current_width = 0;
        }
    }
    if (current_width != 0)
        printf("\n");
}

/* Display long listing */
void displayLongList(char **files, int count, const char *path, bool colorize) {
    (void)colorize; // suppress unused warning
    struct stat statbuf;
    char fullpath[MAX_NAME * 2];

    for (int i = 0; i < count; i++) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, files[i]);
        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        /* Permissions */
        char perms[11] = "----------";
        if (S_ISDIR(statbuf.st_mode)) perms[0] = 'd';
        if (S_ISLNK(statbuf.st_mode)) perms[0] = 'l';
        if (statbuf.st_mode & S_IRUSR) perms[1] = 'r';
        if (statbuf.st_mode & S_IWUSR) perms[2] = 'w';
        if (statbuf.st_mode & S_IXUSR) perms[3] = 'x';
        if (statbuf.st_mode & S_IRGRP) perms[4] = 'r';
        if (statbuf.st_mode & S_IWGRP) perms[5] = 'w';
        if (statbuf.st_mode & S_IXGRP) perms[6] = 'x';
        if (statbuf.st_mode & S_IROTH) perms[7] = 'r';
        if (statbuf.st_mode & S_IWOTH) perms[8] = 'w';
        if (statbuf.st_mode & S_IXOTH) perms[9] = 'x';

        printf("%s %2ld %2ld %6ld ", perms, (long)statbuf.st_nlink,
               (long)statbuf.st_uid, (long)statbuf.st_size);
        printFileWithColor(path, files[i]);
        printf("\n");
    }
}

/* Print filename with color based on type */
void printFileWithColor(const char *path, const char *filename) {
    struct stat statbuf;
    char fullpath[MAX_NAME * 2];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);

    if (lstat(fullpath, &statbuf) == -1) {
        perror("lstat");
        printf("%s  ", filename);
        return;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        printf(COLOR_BLUE "%s" COLOR_RESET "  ", filename);
    } else if (S_ISLNK(statbuf.st_mode)) {
        printf(COLOR_PINK "%s" COLOR_RESET "  ", filename);
    } else if (statbuf.st_mode & S_IXUSR) {
        printf(COLOR_GREEN "%s" COLOR_RESET "  ", filename);
    } else if (strstr(filename, ".tar") || strstr(filename, ".gz") || strstr(filename, ".zip")) {
        printf(COLOR_RED "%s" COLOR_RESET "  ", filename);
    } else if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode) ||
               S_ISFIFO(statbuf.st_mode) || S_ISSOCK(statbuf.st_mode)) {
        printf(COLOR_REVERSE "%s" COLOR_RESET "  ", filename);
    } else {
        printf("%s  ", filename);
    }
}
