# REPORT.md — Feature 4: Horizontal Column Display (-x Option)

## 📘 Version: v1.3.0  
**Feature:** Horizontal Column Display (-x)  
**Instructor:** Dr. Muhammad Arif Butt  

---

### **Q1. Compare the implementation complexity of the "down then across" (vertical) printing logic versus the "across" (horizontal) printing logic. Which one requires more pre-calculation and why?**

The **vertical ("down then across")** printing logic is more complex because it must:
- Calculate how many files fit per column and how many rows are needed.
- Determine the index of each file in a grid-like structure.
- Maintain alignment across columns for consistent formatting.

The **horizontal ("across")** logic is simpler:
- Files are printed left to right.
- A newline is printed only when the current line reaches the terminal width.
- Only column width and cursor position are tracked.

✅ **Conclusion:**  
The **vertical layout** requires more **pre-calculation** and index management, while the **horizontal layout** is simpler and requires only **width tracking**.

---

### **Q2. Describe the strategy you used in your code to manage the different display modes (-l, -x, and default). How did your program decide which function to call for printing?**

The program uses a **state flag** to track the display mode:
- `0` → Default vertical  
- `1` → Long listing (-l)  
- `2` → Horizontal (-x)

The flag is set using `getopt()` argument parsing:
```c
while ((opt = getopt(argc, argv, "lx")) != -1) {
    switch (opt) {
        case 'l': display_mode = 1; break;
        case 'x': display_mode = 2; break;
        default:  display_mode = 0; break;
    }
}
