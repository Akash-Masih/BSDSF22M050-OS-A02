/*
 * ls-v1.2.0
 * Feature: Column Display (Down Then Across)
 * Author: Akash Masih 
 *
 * Usage:
 *   ./bin/ls-v1.2.0         -> multi-column default listing (down then across)
 *   ./bin/ls-v1.2.0 <dir>   -> list contents of <dir>
 *
 * Notes:
 *  - Hidden files (names starting with '.') are skipped, matching assignment.
 *  - Terminal width obtained with ioctl(TIOCGWINSZ); fallback 80 columns.
 */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define DEFAULT_TERM_WIDTH 80
#define COL_SPACING 2       /* spaces between columns */

static int get_term_width(void) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return (int)w.ws_col;
    }
    return DEFAULT_TERM_WIDTH;
}

/* read directory entries into dynamic array (skip entries starting with '.') */
static char **read_dir_entries(const char *dirpath, size_t *out_count, size_t *out_maxlen) {
    DIR *dp = opendir(dirpath);
    if (!dp) {
        fprintf(stderr, "Cannot open directory: %s: %s\n", dirpath, strerror(errno));
        *out_count = 0;
        *out_maxlen = 0;
        return NULL;
    }

    size_t cap = 64;
    size_t n = 0;
    size_t maxlen = 0;
    char **names = malloc(cap * sizeof(char*));
    if (!names) { perror("malloc"); closedir(dp); return NULL; }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue; /* skip hidden files */
        size_t len = strlen(entry->d_name);
        char *copy = malloc(len + 1);
        if (!copy) { perror("malloc"); break; }
        strcpy(copy, entry->d_name);

        if (n >= cap) {
            cap *= 2;
            char **tmp = realloc(names, cap * sizeof(char*));
            if (!tmp) { perror("realloc"); free(copy); break; }
            names = tmp;
        }
        names[n++] = copy;
        if (len > maxlen) maxlen = len;
    }
    closedir(dp);

    /* shrink to fit */
    if (n < cap) {
        char **tmp = realloc(names, n * sizeof(char*));
        if (tmp) names = tmp;
    }

    *out_count = n;
    *out_maxlen = maxlen;
    return names;
}

static void free_entries(char **names, size_t n) {
    if (!names) return;
    for (size_t i = 0; i < n; ++i) free(names[i]);
    free(names);
}

/* compare for qsort (alphabetical) */
static int name_cmp(const void *a, const void *b) {
    const char * const *pa = a;
    const char * const *pb = b;
    return strcmp(*pa, *pb);
}

int main(int argc, char *argv[]) {
    const char *dirpath = ".";
    if (argc > 1) dirpath = argv[1];

    size_t nfiles = 0, maxlen = 0;
    char **names = read_dir_entries(dirpath, &nfiles, &maxlen);
    if (names == NULL) {
        /* error message already printed */
        return (nfiles == 0) ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    if (nfiles == 0) {
        free_entries(names, nfiles);
        return EXIT_SUCCESS;
    }

    /* sort names alphabetically (like ls) */
    qsort(names, nfiles, sizeof(char*), name_cmp);

    int term_width = get_term_width();
    /* column width is longest name + spacing */
    size_t col_width = maxlen + COL_SPACING;
    if (col_width == 0) col_width = 1;

    int ncols = term_width / (int)col_width;
    if (ncols < 1) ncols = 1;

    /* compute rows: ceil(nfiles / ncols) */
    int nrows = (int)((nfiles + ncols - 1) / ncols);

    /* Print row by row (down then across):
       for r in 0..nrows-1:
         for c in 0..ncols-1:
           idx = c*nrows + r
    */
    for (int r = 0; r < nrows; ++r) {
        for (int c = 0; c < ncols; ++c) {
            int idx = c * nrows + r;
            if (idx >= (int)nfiles) continue;
            /* For last column, no extra spacing required */
            if (c == ncols - 1) {
                printf("%s", names[idx]);
            } else {
                /* pad to col_width (left aligned) */
                printf("%-*s", (int)col_width, names[idx]);
            }
        }
        printf("\n");
    }

    free_entries(names, nfiles);
    return EXIT_SUCCESS;
}
