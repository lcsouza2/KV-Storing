
typedef struct sstable {
    char *path;
    int min_key;
    int max_key;
    int level;
} sstable_t;

sstable_t *create_sstable(int min_key, int max_key, int level);