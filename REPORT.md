# 🧾 REPORT.md
### **BSDSF22M050 — Operating Systems Assignment 02**
### **Task 5: Tag and Release Version 1.1.0 — Long Listing Format**

---

## **Q1: What is the crucial difference between the `stat()` and `lstat()` system calls?**
The **main difference** between `stat()` and `lstat()` lies in how they handle **symbolic links (symlinks)**.

| **Function** | **Behavior on Symbolic Links** |
|---------------|--------------------------------|
| `stat()` | Follows the link and returns information about the **target file** the link points to. |
| `lstat()` | Returns information about the **link itself** (it does not follow the link). |

### **Explanation**
- `stat()` is used when you need information about the **actual file**.
- `lstat()` is used when you want details about the **symlink itself**, such as its permissions, size, or modification time.

#### **Example**
```bash
ln -s original.txt link.txt
