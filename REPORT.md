# Custom LS Command – Full Report

Author: Akash Masih (BSDSF22M050)  
Project: OS Assignment – Custom LS Versions v1.0.0 to v1.6.0  

---

## Feature v1.0.0 – Basic LS Implementation
**Report Questions:**  
1. **What system call is used to read the contents of a directory?**  
   - The `opendir()` function is used to open a directory stream, and `readdir()` is used to read entries from it sequentially.

2. **What data structure is used to store the filenames before printing?**  
   - An array of strings (`char *filenames[MAX_FILES]`) is used to store directory entries temporarily before sorting or displaying.

---

## Feature v1.1.0 – Alphabetical Sorting
**Report Questions:**  
1. **How are files sorted alphabetically in C?**  
   - The `qsort()` function is used with a comparison function that calls `strcmp()` on filenames.

2. **What is the purpose of the compare function in qsort?**  
   - The compare function defines the order of sorting. For alphabetical sorting, it returns a negative, zero, or positive value depending on the lexical order of two strings.

---

## Feature v1.2.0 – Long Listing (-l Option)
**Report Questions:**  
1. **Which `stat` fields are used to display file permissions, size, and number of links?**  
   - `st_mode` → permissions and file type  
   - `st_nlink` → number of hard links  
   - `st_uid` → owner ID  
   - `st_gid` → group ID  
   - `st_size` → file size in bytes  

2. **Explain how permissions are extracted from st_mode.**  
   - Bitwise AND operations with `S_IRUSR`, `S_IWUSR`, `S_IXUSR` etc. determine whether a user, group, or others have read, write, or execute permissions.

---

## Feature v1.3.0 – Horizontal Columns (-x Option)
**Report Questions:**  
1. **How do you calculate the number of columns to display files horizontally?**  
   - Determine the terminal width (`ioctl` with `TIOCGWINSZ`) and the maximum filename length. Divide terminal width by column width to get the number of columns.

2. **What happens if a filename is longer than the column width?**  
   - The column width is adjusted dynamically based on the longest filename, ensuring proper alignment.

---

## Feature v1.4.0 – User/Group Name Display
**Report Questions:**  
1. **Which functions are used to map UID and GID to human-readable names?**  
   - `getpwuid(statbuf.st_uid)` → returns owner username  
   - `getgrgid(statbuf.st_gid)` → returns group name

2. **Why is it important to display user/group names instead of numeric IDs?**  
   - Numeric IDs are not meaningful to users. Displaying usernames and group names improves readability and usability.

---

## Feature v1.5.0 – Colorized Output Based on File Type
**Report Questions:**  

1. **How do ANSI escape codes work to produce color in a standard Linux terminal? Show the specific code sequence for printing text in green.**  
   - ANSI escape codes are special sequences that the terminal interprets to change text color, background, or style.  
   - Example for green text:  
     ```c
     printf("\033[0;32m%s\033[0m", filename);
     ```
     - `\033[0;32m` → sets text color to green  
     - `\033[0m` → resets color to default  

2. **To color an executable file, which permission bits in st_mode need to be checked?**  
   - The executable bits are checked using bitwise AND operations:  
     - Owner executable: `statbuf.st_mode & S_IXUSR`  
     - Group executable: `statbuf.st_mode & S_IXGRP`  
     - Others executable: `statbuf.st_mode & S_IXOTH`  
   - If any of these bits are set, the file is considered executable and can be displayed in green.

---

## Feature v1.6.0 – Recursive Listing (-R Option)
**Report Questions:**  

1. **In a recursive function, what is a "base case"? In the context of your recursive ls, what is the base case that stops the recursion from continuing forever?**  
   - A **base case** is a condition that stops recursion to prevent infinite calls.  
   - In our `ls -R` implementation, the base case occurs when a directory contains no subdirectories (or only `.` and `..`). At this point, no further recursive calls are made.

2. **Why is it essential to construct a full path before making a recursive call? What would happen if you simply called `do_ls("subdir")` from within `do_ls("parent_dir")`?**  
   - Constructing the full path (`parent_dir/subdir`) ensures the program accesses the correct subdirectory relative to the current directory being listed.  
   - If only `do_ls("subdir")` is called, it will search for `subdir` in the **current working directory** instead of `parent_dir`, causing recursion to fail and subdirectories to be skipped.  

---

**End of Report**
