typedef struct {
    char *key;
    char *value;
    int source_index;
} HeapNode;

typedef struct {
    HeapNode **nodes;
    int size;
    int capacity;
    int (*compare_func)(const void *, const void *);
} MinHeap;

MinHeap *create_min_heap(int capacity);
HeapNode *create_node_from_key_value_pair(char *key, char *value);
HeapNode *heap_replace_root(MinHeap *heap, HeapNode *new_root);
HeapNode *heap_extract_min(MinHeap *heap);
void free_min_heap(MinHeap *heap);
void build_min_heap(MinHeap *heap);

