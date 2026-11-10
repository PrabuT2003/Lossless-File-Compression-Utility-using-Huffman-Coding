# Lossless File Compression Utility

## Abstract
This project implements a lossless file compression utility using Huffman Coding, demonstrating the Greedy Algorithm paradigm from the IF3151 Advanced Data Structures and Algorithms syllabus. It reduces the storage footprint of text files by analyzing character frequencies and assigning variable-length binary prefix codes to maximize storage efficiency without data corruption. 

## Technical Architecture
*   **Data Structures:** Min-Heap (Priority Queue), Binary Tree (Huffman Tree)
*   **Language:** C
*   **Memory Management:** Dynamic allocation with strict lifecycle tracking to prevent memory leaks during deep tree traversals.

## Compilation Instructions
To build the executables, navigate to the project directory in your terminal and compile the source files using GCC:
```bash
gcc -Wall -O2 -o compress compress.c huffman.c
gcc -Wall -O2 -o decompress decompress.c huffman.c