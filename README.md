# Lossless File Compression Utility 🗜️

A high-performance, memory-safe file compression and decompression engine written entirely in C. This utility implements **Huffman Coding**—a greedy algorithmic approach that assigns variable-length binary prefix codes to characters based on their statistical frequency.

This project was developed as a practical application of hierarchical data structures and optimal prefix-routing for the **IF3151 Advanced Data Structures and Algorithms** coursework.

## 🚀 Features

* **True Bitwise Packing:** Standard file I/O operations waste space by storing '0's and '1's as full bytes. This utility uses bitwise shifts and buffers to pack exactly 8 bits into a single byte before writing to the disk, ensuring genuine file size reduction.
* **Header Serialization:** Automatically embeds the character frequency table into the compressed binary file, allowing the decompressor to flawlessly reconstruct the exact Min-Heap and Huffman tree on the fly.
* **Lossless Restoration:** Guarantees 100% data integrity. The decompressed output matches the original input byte-for-byte.
* **Memory Safe:** Built with strict lifecycle tracking. Valgrind profiling confirms 0 memory leaks across all dynamic node allocations and recursive tree traversals.

## 🛠️ Technical Architecture

* **Language:** C
* **Data Structures:** Min-Heap (Priority Queue), Binary Tree (Huffman Tree), Hash Map (Frequency Array)
* **Algorithms:** Greedy Approach, Bottom-Up Tree Construction, Post-Order Traversal (Memory Management)

## 📂 Repository Structure

* `huffman.h` / `huffman.c` - Core data structures, Min-Heap logic, and Huffman Tree construction.
* `compress.c` - Main driver for frequency mapping, code generation, bit-packing, and binary file writing.
* `decompress.c` - Rebuilds the tree from the serialized header and decodes the packed bitstream.
* `Makefile` - Automates the GCC compilation process.
* `Sample_input.txt` - Test dataset for evaluating compression ratios.

## ⚙️ Compilation & Usage

### 1. Build the Executables
Ensure you have `gcc` installed, then run the Makefile from your terminal:
```bash
make

Compress a File
./compress Sample_input.txt compressed_output.bin
The terminal will output the original size, the new compressed size, and the total space saved.

Decompress a File
./decompress compressed_output.bin restored_output.txt
The restored file will perfectly match the exact byte size of the original input.

📊 Asymptotic Complexity

Phase Time Complexity Space Complexity
Frequency Mapping O(N) O(U)
Min-Heap Construction O(U \log U) O(U)
Tree Traversal O(U) O(U)
Total Practical Complexity O(N) O(U)

Note: N represents the total number of characters in the file. U represents the unique character set (bounded by a constant maximum of 256 for standard ASCII), making the algorithm highly scalable and memory-efficient.
