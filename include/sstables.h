#ifndef SSTABLES_H
#define SSTABLES_H
#endif
#include "memtable.h"
#include "interface.h"
#include "bloom_filter.h"

typedef struct sstable {
    char *path;
    char *min_key;
    char *max_key;
    int level;
    BloomFilter *bloom_filter;
} SSTable;

typedef struct level_index {
    int count;
    int capacity;
    SSTable **tables;
} LevelIndex;

int flush_memtable_to_disk(Memtable *memtable, int level);
KeyValue *search_in_sstables(char *key);
