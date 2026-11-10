#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

void compressFile(const char* inputFile, const char* outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    if (!inFile) {
        perror("Error opening input file");
        return;
    }

    // 1. Calculate character frequencies
    unsigned freq[256] = {0};
    int ch;
    unsigned originalSize = 0;
    
    while ((ch = fgetc(inFile)) != EOF) {
        freq[ch]++;
        originalSize++;
    }

    // Count unique characters to size our MinHeap
    int uniqueChars = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) uniqueChars++;
    }

    if (uniqueChars == 0) {
        printf("File is empty or could not be read.\n");
        fclose(inFile);
        return;
    }

    // Populate arrays for tree construction
    char data[uniqueChars];
    unsigned compressedFreq[uniqueChars];
    int idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[idx] = (char)i;
            compressedFreq[idx] = freq[i];
            idx++;
        }
    }

    // 2. Build Huffman Tree and Generate Dictionary Codes
    HuffmanNode* root = buildHuffmanTree(data, compressedFreq, uniqueChars);
    int arr[256], top = 0;
    char codes[256][256] = {{0}};
    generateCodes(root, arr, top, codes);

    // 3. Write Compressed Output
    FILE *outFile = fopen(outputFile, "wb");
    if (!outFile) {
        perror("Error opening output file");
        fclose(inFile);
        return;
    }

    // Write Header: Original size followed by the frequency table
    // (Required for the decompressor to rebuild the exact same tree)
    fwrite(&originalSize, sizeof(unsigned), 1, outFile);
    fwrite(freq, sizeof(unsigned), 256, outFile);
    
    unsigned compressedSize = sizeof(unsigned) * 257; // Header overhead

    // 4. Bitwise Packing
    rewind(inFile);
    unsigned char bitBuffer = 0;
    int bitCount = 0;

    while ((ch = fgetc(inFile)) != EOF) {
        char* code = codes[(unsigned char)ch];
        for (int i = 0; code[i] != '\0'; i++) {
            bitBuffer = (bitBuffer << 1) | (code[i] - '0');
            bitCount++;
            
            // Once buffer is full (8 bits), write to file
            if (bitCount == 8) {
                fputc(bitBuffer, outFile);
                compressedSize++;
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }

    // Flush any remaining bits in the buffer (padded with zeros)
    if (bitCount > 0) {
        bitBuffer = bitBuffer << (8 - bitCount);
        fputc(bitBuffer, outFile);
        compressedSize++;
    }

    fclose(inFile);
    fclose(outFile);
    freeHuffmanTree(root);

    // 5. Output Execution Statistics
    printf("Compression successful.\n");
    printf("------------------------------------\n");
    printf("Original Size:   %u bytes\n", originalSize);
    printf("Compressed Size: %u bytes\n", compressedSize);
    
    if (originalSize > 0) {
        double ratio = (1.0 - ((double)compressedSize / originalSize)) * 100;
        printf("Space Saved:     %.2f%%\n", ratio);
        if (compressedSize > originalSize) {
            printf("\nNote: File size increased due to header overhead on a very small input file.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    compressFile(argv[1], argv[2]);
    return 0;
}