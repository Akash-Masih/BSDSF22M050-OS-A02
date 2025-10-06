# LS Command Feature v1.6.0 – Recursive Listing (-R)

## Report Questions

### 1. In a recursive function, what is a "base case"? In the context of your recursive ls, what is the base case that stops the recursion from continuing forever?

A **base case** in a recursive function is a condition that stops the recursion. Without a base case, the function would call itself indefinitely, eventually causing a stack overflow.

**In the context of our recursive `ls` (`ls-v1.6.0`)**:
- The base case occurs when the function encounters a directory that **has no subdirectories** (or only contains `.` and `..`).  
- In this case, the recursion does not proceed further for that directory, and the function returns to the previous call level.  

This ensures that recursion terminates correctly and every directory is processed exactly once.

---

### 2. Explain why it is essential to construct a full path (e.g., "parent_dir/subdir") before making a recursive call. What would happen if you simply called `do_ls("subdir")` from within the `do_ls("parent_dir")` function call?

Constructing a **full path** is essential because the recursive call must know the **absolute or relative path** from the current working directory to the subdirectory.  

If we only call `do_ls("subdir")`:
- The function would look for `subdir` in the **current working directory**, not inside `parent_dir`.  
- This would fail unless the current working directory happens to be the same as the parent directory being listed.  
- As a result, many subdirectories would be skipped, and the recursion would not correctly traverse the directory tree.

By using `parent_dir/subdir`:
- The program accurately navigates into nested directories regardless of the current working directory.  
- All nested files and subdirectories are listed recursively, ensuring correct behavior.

