#include "k_way.h"
#include <string.h>
#include <stdlib.h>


static int _compare_heap_nodes(HeapNode *node1, HeapNode *node2) {
    int cmp = strcmp(node1->key, node2->key);
    if (cmp != 0) {
        return cmp;
    }

    if (node1->level != node2->level) {
        return (node1->level - node2->level);
    }
    return (node2->sstable_id - node1->sstable_id);
}

static SSTableIterator *_create_iterator() {
    SSTableIterator *iterator = malloc(sizeof(SSTableIterator));
    if (!iterator) return NULL;
    iterator->sstable_id = 0;
    iterator->file = NULL;
    iterator->current_key = NULL;
    iterator->current_value = NULL;
    iterator->is_exhausted = 0;
    return iterator;
}

static SSTableIterator *_iterator_init(const char *file_path, int sstable_id) {
    FILE *fp = fopen(file_path, "rb");
    if (!fp) return NULL;

    int len;
    if (fread(&len, sizeof(int), 1, fp) == 1 && len > 0) {
        fseek(fp, len, SEEK_CUR);
    }
    if (fread(&len, sizeof(int), 1, fp) == 1 && len > 0) {
        fseek(fp, len, SEEK_CUR);
    }

    SSTableIterator *iterator = _create_iterator();
    if (!iterator) {
        fclose(fp);
        return NULL;
    }

    iterator->sstable_id = sstable_id;
    iterator->file = fp;
    return iterator;
}

static int _iterator_next(SSTableIterator *iterator) {
    if (iterator->is_exhausted) return -1;

    if (iterator->current_key) free(iterator->current_key);
    if (iterator->current_value) free(iterator->current_value);
    iterator->current_key = NULL;
    iterator->current_value = NULL;


    int key_len;
    if (fread(&key_len, sizeof(int), 1, iterator->file) != 1) {
        iterator->is_exhausted = 1;
        return -1;
    }

    if (key_len > 0) {
        iterator->current_key = malloc(key_len + 1);
        fread(iterator->current_key, sizeof(char), key_len, iterator->file);
        iterator->current_key[key_len] = '\0';
    }

    int value_len;
    fread(&value_len, sizeof(int), 1, iterator->file);
    if (value_len > 0) {
        iterator->current_value = malloc(value_len + 1);
        fread(iterator->current_value, sizeof(char), value_len, iterator->file);
        iterator->current_value[value_len] = '\0';
    }

    return 0;
}

static HeapNode *_create_node_from_iterator(SSTableIterator *iterator) {
    HeapNode *node = malloc(sizeof(HeapNode));
    node->key = strdup(iterator->current_key);
    node->value = iterator->current_value ? strdup(iterator->current_value) : NULL;
    node->iterator_index = iterator->sstable_id;
    node->level = 0; // Ajuste conforme seu esquema de prioridade
    return node;
}

static void _swap_nodes(HeapNode **a, HeapNode **b) {
    HeapNode *temp = *a;
    *a = *b;
    *b = temp;
}

static void _shift_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent_index = (index - 1) / 2;
        if (_compare_heap_nodes(heap->nodes[index], heap->nodes[parent_index]) < 0) {
            _swap_nodes(&heap->nodes[index], &heap->nodes[parent_index]);
            index = parent_index;
        } else {
            break;
        }
    }
}

static void _shift_down(MinHeap *heap, int index) {
    int size = heap->size;
    while (1) {
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;
        int smallest = index;

        if (left_child < size && _compare_heap_nodes(heap->nodes[left_child], heap->nodes[smallest]) < 0) {
            smallest = left_child;
        }
        if (right_child < size && _compare_heap_nodes(heap->nodes[right_child], heap->nodes[smallest]) < 0) {
            smallest = right_child;
        }
        if (smallest != index) {
            _swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

static HeapNode *_heap_pop_and_replace(MinHeap *heap, SSTableIterator **iterators) {
    HeapNode *min_node = heap->nodes[0];

    int idx = min_node->iterator_index;
    if (_iterator_next(iterators[idx]) == 0) {
        heap->nodes[0] = _create_node_from_iterator(iterators[idx]);
    } else {
        heap->nodes[0] = heap->nodes[heap->size - 1];
        heap->size--;
    }

    if (heap->size > 0) {
        _shift_down(heap, 0);
    }
    return min_node;
}

static MinHeap *_create_min_heap(int capacity) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->nodes = malloc(sizeof(HeapNode *) * capacity);
    if (!heap->nodes) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

MinHeap *min_heap_init(SSTableIterator **iterators, int num_iterators) {
    MinHeap *heap = _create_min_heap(num_iterators);

    for (int i = 0; i < num_iterators; i++) {
        if (_iterator_next(iterators[i]) == 0) {
            HeapNode *node = _create_node_from_iterator(iterators[i]);
            heap->nodes[heap->size++] = node;
        }
    }

    for (int i = (heap->size / 2) - 1; i >= 0; i--) {
        _shift_down(heap, i);
    }
    return heap;
}

HeapNode *get_next_merged_kv(MinHeap *heap, SSTableIterator **iterators) {
    if (heap->size == 0) return NULL;

    HeapNode *winner = heap->nodes[0];

    _heap_pop_and_replace(heap, iterators);

    while (heap->size > 0 && strcmp(winner->key, heap->nodes[0]->key) == 0) {
        HeapNode *old = _heap_pop_and_replace(heap, iterators);
        free(old->key);
        free(old->value);
        free(old);
    }

    return winner;
}

void compact_sstables(SSTableIterator **iters, int num_iters, const char *output_file) {
    MinHeap *heap = min_heap_init(iters, num_iters);
    FILE *out = fopen(output_file, "wb");

    HeapNode *key_value;
    while ((key_value = get_next_merged_kv(heap, iters)) != NULL) {
        int key_len = strlen(key_value->key);
        int value_len = key_value->value ? strlen(key_value->value) : -1;

        fwrite(&key_len, sizeof(int), 1, out);
        fwrite(key_value->key, sizeof(char), key_len, out);
        fwrite(&value_len, sizeof(int), 1, out);
        if (value_len > 0) fwrite(key_value->value, sizeof(char), value_len, out);

        free(key_value->key);
        free(key_value->value);
        free(key_value);
    }

    fclose(out);
    free(heap->nodes);
    free(heap);
}
