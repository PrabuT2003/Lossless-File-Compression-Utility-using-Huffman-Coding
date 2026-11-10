#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

void decompressFile(const char* inputFile, const char* outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    if (!inFile) {
        perror("Error opening compressed file");
        return;
    }

    // 1. Read Header Details
    unsigned originalSize = 0;
    unsigned freq[256] = {0};
    
    if (fread(&originalSize, sizeof(unsigned), 1, inFile) != 1) {
        printf("Error reading original file size.\n");
        fclose(inFile);
        return;
    }
    
    if (fread(freq, sizeof(unsigned), 256, inFile) != 256) {
        printf("Error reading frequency table.\n");
        fclose(inFile);
        return;
    }

    // Handle empty file case
    if (originalSize == 0) {
        FILE *outFile = fopen(outputFile, "wb");
        if (outFile) fclose(outFile);
        fclose(inFile);
        printf("Decompressed empty file successfully.\n");
        return;
    }

    // 2. Rebuild the exact same Huffman Tree
    int uniqueChars = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) uniqueChars++;
    }

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

    HuffmanNode* root = buildHuffmanTree(data, compressedFreq, uniqueChars);

    // 3. Prepare for output
    FILE *outFile = fopen(outputFile, "wb");
    if (!outFile) {
        perror("Error opening output file");
        fclose(inFile);
        freeHuffmanTree(root);
        return;
    }

    // 4. Decode the bitstream
    HuffmanNode* curr = root;
    unsigned char bitBuffer;
    unsigned bytesWritten = 0;

    // Read byte by byte
    while (fread(&bitBuffer, 1, 1, inFile) == 1) {
        // Read bit by bit from left to right (MSB to LSB)
        for (int i = 7; i >= 0; i--) {
            int bit = (bitBuffer >> i) & 1;

            if (bit == 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }

            if (isLeaf(curr)) {
                fputc(curr->data, outFile);
                bytesWritten++;
                curr = root; // Reset to root for next character

                // Stop exactly when we've decoded the original number of bytes
                // This ignores the padding bits at the very end of the file
                if (bytesWritten == originalSize) {
                    break;
                }
            }
        }
        if (bytesWritten == originalSize) {
            break;
        }
    }

    fclose(inFile);
    fclose(outFile);
    freeHuffmanTree(root);

    printf("Decompression successful.\n");
    printf("Restored File Size: %u bytes\n", bytesWritten);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <compressed_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    decompressFile(argv[1], argv[2]);
    return 0;
}