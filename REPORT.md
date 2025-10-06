## Feature-3: Column Display — REPORT

### Q1: Explain the general logic for printing items in a "down then across" columnar format. Why is a simple single loop through the list of filenames insufficient?

**General logic (down then across):**
1. First read *all* filenames into an array and determine the length of the longest filename.
2. Compute the terminal width and derive the column width = (longest filename length + spacing).
3. Compute how many columns (`num_cols`) fit: `num_cols = terminal_width / column_width` (minimum 1).
4. Compute number of rows needed: `num_rows = ceil(total_files / num_cols)`.
5. Print row-by-row. For each row `r` (0..num_rows-1), print items at indices:
   - `r`, `r + num_rows`, `r + 2*num_rows`, ... up to `r + (num_cols-1)*num_rows`
   This maps the 1D array into a matrix that fills columns top-to-bottom (down) before moving to the next column (across).

**Why a single loop is insufficient:**
- A single loop that simply prints filenames sequentially will fill rows left-to-right (across) first (or produce a single column), not the required **down-then-across** ordering.
- To achieve down-then-across, the mapping of index → printed position is non-linear: the `i`th printed item in row-major order corresponds to a different index in the filenames array.
- You must know total count and longest filename ahead of printing to calculate layout parameters (columns and rows) — hence pre-gathering filenames is mandatory.

---

### Q2: What is the purpose of the `ioctl` system call in this context? What are the limitations if you only use a fixed-width fallback?

**Purpose of `ioctl(TIOCGWINSZ`)**:
- It queries the terminal driver for the current terminal window size (rows and columns).
- Using `TIOCGWINSZ` allows the program to adapt its output to the real terminal width so columns fit properly and maximize use of horizontal space.

**Limitations of fixed-width fallback (e.g., 80 columns):**
- Modern terminals can be resized; a fixed width may under-utilize or overflow the available width.
- On wide screens, fixed width wastes space and results in fewer columns than possible.
- On narrow terminals, fixed width can cause wrapping or misalignment.
- User experience is degraded (not responsive). Detecting the real width via `ioctl` ensures output adapts to the user's environment.

