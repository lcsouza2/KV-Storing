#include <string.h>
#include "min_heap.h"
#include "settings.h"
#include "logging.h"


static HeapNode *_create_node() {
    HeapNode *node = malloc(sizeof(HeapNode));
    node->key = NULL;
    node->value = NULL;
    node->source_index = -1;
    return node;
}

static void _swap_nodes(HeapNode **a, HeapNode **b) {
    HeapNode *temp = *a;
    *a = *b;
    *b = temp;
}

static void _shift_down(MinHeap *heap, int index) {
    int size = heap->size;
    while (1) {
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;
        int smallest = index;

        if (left_child < size && heap->compare_func(heap->nodes[left_child], heap->nodes[smallest]) < 0) {
            smallest = left_child;
        }
        if (right_child < size && heap->compare_func(heap->nodes[right_child], heap->nodes[smallest]) < 0) {
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


HeapNode *create_node_from_key_value_pair(char *key, char *value) {
    HeapNode *node = _create_node();
    if (!node) return NULL;

    node->key = key ? strdup(key) : NULL;
    node->value = value ? strdup(value) : NULL;
    return node;
}

HeapNode *heap_extract_min(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    HeapNode *min_node = heap->nodes[0];

    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    if (heap->size > 0) _shift_down(heap, 0);
    return min_node;
}

HeapNode *heap_replace_root(MinHeap *heap, HeapNode *new_root) {
    HeapNode *old_root = heap->nodes[0];
    heap->nodes[0] = new_root;
    _shift_down(heap, 0);
    return old_root;
}

MinHeap *create_min_heap(int capacity) {
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

void free_min_heap(MinHeap *heap) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++) {
        free(heap->nodes[i]->key);
        if (heap->nodes[i]->value) free(heap->nodes[i]->value);
        free(heap->nodes[i]);
    }
    free(heap->nodes);
    free(heap);
}

void build_min_heap(MinHeap *heap) {
    for (int i = (heap->size / 2) - 1; i >= 0; i--) {
        _shift_down(heap, i);
    }
}
