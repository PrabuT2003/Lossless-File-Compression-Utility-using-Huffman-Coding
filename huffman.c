#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

HuffmanNode* createNode(char data, unsigned freq) {
    HuffmanNode* temp = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    return minHeap;
}

void freeHuffmanTree(HuffmanNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

void freeMinHeap(MinHeap* minHeap) {
    if (minHeap == NULL) return;
    free(minHeap->array);
    free(minHeap);
}

void swapNodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

HuffmanNode* extractMin(MinHeap* minHeap) {
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, HuffmanNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

HuffmanNode* buildHuffmanTree(char data[], unsigned freq[], int size) {
    HuffmanNode *left, *right, *top;
    MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // '$' is a special value for internal nodes, not used during decoding
        top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    HuffmanNode* root = extractMin(minHeap);
    freeMinHeap(minHeap);
    return root;
}

int isLeaf(HuffmanNode* root) {
    return !(root->left) && !(root->right);
}

void generateCodes(HuffmanNode* root, int arr[], int top, char codes[][256]) {
    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1, codes);
    }

    if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1, codes);
    }

    if (isLeaf(root)) {
        for (int i = 0; i < top; ++i) {
            codes[(unsigned char)root->data][i] = arr[i] ? '1' : '0';
        }
        codes[(unsigned char)root->data][top] = '\0';
    }
}