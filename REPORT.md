# Report for ls-v1.5.0 – Colorized Output

**Author:** Akash Masih (BSDSF22M050)  
**Feature:** Colorized Output Based on File Type  
**Version:** 1.5.0  

---

## 1. How do ANSI escape codes work to produce color in a standard Linux terminal?

ANSI escape codes are special sequences of characters that the terminal interprets to apply text formatting, colors, or other display effects. They allow programs like `ls` to produce colored or styled output in the terminal.  

The general format of an ANSI escape code is:


Where:  
- `\033` is the escape character (octal 033)  
- `[` starts the control sequence  
- `<code>` is one or more numeric codes separated by semicolons  
- `m` ends the sequence and applies the style  

### Example: Printing text in green

```c
printf("\033[0;32mThis text is green\033[0m\n");
```

## How to color an executable file using permission bits

In Linux, each file has a **mode field** called `st_mode` inside the `struct stat` structure. This field contains information about:

- The **file type** (directory, regular file, symbolic link, etc.)  
- The **permission bits** for owner, group, and others  

To determine if a file is executable, we need to check specific permission bits:

| Permission Bit | Meaning                          |
|----------------|----------------------------------|
| `S_IXUSR`      | Executable by the **owner**      |
| `S_IXGRP`      | Executable by the **group**      |
| `S_IXOTH`      | Executable by **others/world**   |

These macros are defined in `<sys/stat.h>` and can be used to test the executable permission for a file.  

### Example Code

```c
#include <stdio.h>
#include <sys/stat.h>

int main() {
    struct stat statbuf;
    if (stat("myfile", &statbuf) == -1) {
        perror("stat");
        return 1;
    }

    if (statbuf.st_mode & S_IXUSR) {
        printf("Executable by owner\n");
    }
    if (statbuf.st_mode & S_IXGRP) {
        printf("Executable by group\n");
    }
    if (statbuf.st_mode & S_IXOTH) {
        printf("Executable by others\n");
    }

    if (statbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        printf("File is executable (color it green in ls-v1.5.0)\n");
    } else {
        printf("File is not executable\n");
    }

    return 0;
}
