#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

// Represents a single node in the Huffman Tree
typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// Represents the Min-Heap priority queue
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode** array;
} MinHeap;

// Memory Allocation and Cleanup
HuffmanNode* createNode(char data, unsigned freq);
MinHeap* createMinHeap(unsigned capacity);
void freeHuffmanTree(HuffmanNode* root);
void freeMinHeap(MinHeap* minHeap);

// Min-Heap Priority Queue Operations
void swapNodes(HuffmanNode** a, HuffmanNode** b);
void minHeapify(MinHeap* minHeap, int idx);
int isSizeOne(MinHeap* minHeap);
HuffmanNode* extractMin(MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, HuffmanNode* minHeapNode);
void buildMinHeap(MinHeap* minHeap);

// Core Huffman Algorithm
HuffmanNode* buildHuffmanTree(char data[], unsigned freq[], int size);

// Code Generation and Traversal
int isLeaf(HuffmanNode* root);
void generateCodes(HuffmanNode* root, int arr[], int top, char codes[][256]);

#endif // HUFFMAN_H