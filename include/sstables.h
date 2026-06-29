typedef struct sstable {
    char *path;
    char *min_key;
    char *max_key;
    int level;
} SSTable;

SSTable *create_sstable_in_memory(char *min_key, char *max_key, int level);