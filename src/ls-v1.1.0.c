/*
* Programming Assignment 02: ls-v1.1.0
* Feature: Complete Long Listing Format (-l)
* Author: Akash Masih
*/

#define _XOPEN_SOURCE 700   // ✅ Enables getopt(), lstat(), mode_t, etc.
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>      // ✅ for getopt(), optind
#include <pwd.h>         // ✅ for getpwuid()
#include <grp.h>         // ✅ for getgrgid()
#include <time.h>        // ✅ for time formatting
#include <errno.h>       // ✅ for error handling

void do_ls(const char *dirpath, int long_format);
void do_ls_long(const char *dirpath);
void print_permissions(mode_t mode);

int main(int argc, char *argv[]) {
    int opt;
    int long_format = 0;

    // Parse command line options (-l)
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                long_format = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // If no directory is provided, list current directory
    if (optind == argc) {
        do_ls(".", long_format);
    } else {
        for (int i = optind; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], long_format);
            puts("");
        }
    }

    return 0;
}

void do_ls(const char *dirpath, int long_format) {
    if (long_format)
        do_ls_long(dirpath);
    else {
        DIR *dirp;
        struct dirent *dp;

        dirp = opendir(dirpath);
        if (dirp == NULL) {
            perror("opendir");
            return;
        }

        while ((dp = readdir(dirp)) != NULL) {
            if (dp->d_name[0] == '.')
                continue; // Skip hidden files
            printf("%s  ", dp->d_name);
        }
        printf("\n");

        closedir(dirp);
    }
}

void do_ls_long(const char *dirpath) {
    DIR *dirp;
    struct dirent *dp;
    struct stat statbuf;
    char path[1024];

    dirp = opendir(dirpath);
    if (dirp == NULL) {
        perror("opendir");
        return;
    }

    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] == '.')
            continue;

        snprintf(path, sizeof(path), "%s/%s", dirpath, dp->d_name);

        if (lstat(path, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        // Print file type & permissions
        print_permissions(statbuf.st_mode);

        // Print number of links, owner, group, size, time, and name
        printf(" %2lu ", statbuf.st_nlink);

        struct passwd *pwd = getpwuid(statbuf.st_uid);
        struct group *grp = getgrgid(statbuf.st_gid);
        printf("%s %s ", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");

        printf("%8ld ", statbuf.st_size);

        char timebuf[80];
        struct tm *tm_info = localtime(&statbuf.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
        printf("%s ", timebuf);

        printf("%s\n", dp->d_name);
    }

    closedir(dirp);
}

void print_permissions(mode_t mode) {
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
