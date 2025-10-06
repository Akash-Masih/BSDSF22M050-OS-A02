# Feature 5: Alphabetical Sort (v1.4.0)

## 1. Why read all directory entries into memory?

To sort filenames alphabetically, we must have access to the entire list of files at once. Sorting cannot be done incrementally with streaming reads because the relative order of filenames is unknown until all entries are seen.

**Potential drawbacks:**
- High memory usage for directories containing millions of files.
- Possible performance degradation due to dynamic memory allocation overhead.
- Risk of `malloc` failures if memory is exhausted.

## 2. Comparison Function for qsort()

The comparison function allows `qsort()` to know how to order elements in an array. Its signature is:

```c
int compareStrings(const void *a, const void *b);
