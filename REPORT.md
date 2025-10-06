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
