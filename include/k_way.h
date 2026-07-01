#ifndef K_WAY_H
#define K_WAY_H
#include <stdio.h>

typedef struct {
    FILE *file;
    char *current_key;
    char *current_value;
    int level;
    int sstable_id;
    int is_exhausted;
} SSTableIterator;

typedef struct {
    char *key;
    char *value;
    int iterator_index;
    int sstable_id;
    int level;
} HeapNode;

typedef struct {
    HeapNode **nodes;
    int size;
    int capacity;
} MinHeap;

void compact_sstables(SSTableIterator **iters, int num_iters, const char *base_name);

#endif