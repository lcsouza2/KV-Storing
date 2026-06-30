#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "settings.h"
#include "logging.h"
#include "sstables.h"
#include "memtable.h"

LevelIndex _fast_access_sstables[MAX_SSTABLE_LEVELS];

FILE *_open_sstable(SSTable *sstable, const char *mode) {
    if (!sstable || !sstable->path) {
        debug("Invalid SSTable or path.");
        return NULL;
    }
    FILE *file = fopen(sstable->path, mode);
    if (!file) {
        error("Failed to open SSTable file: %s", sstable->path);
        return NULL;
    }
    return file;
}

char *_get_memtable_min_key(Memtable *memtable) {
    if (!memtable || !memtable->root) return NULL;
    AVLNode *current = memtable->root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current->key;
}


char *_get_memtable_max_key(Memtable *memtable) {
    if (!memtable || !memtable->root) return NULL;
    AVLNode *current = memtable->root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current->key;
}


char *_generate_sstable_filepath(int level) {
    char filepath[SSTABLE_MAX_PATH_LENGTH];

    snprintf(
        filepath,
        sizeof(filepath),
        "%s/L%d_%d.dat",
        _get_sstable_path(),
        level,
        _fast_access_sstables[level].count
    );

    // Return a dynamically allocated string to avoid returning a Dangling Pointer since filepath
    // is allocated in the stack and will be deallocated when the function returns.
    return strdup(filepath);
}

void _add_sstable_to_level_index(SSTable *sstable) {
    if (!sstable) {
        debug("SSTable is NULL.");
        return;
    }

    int level = sstable->level;
    if (level < 0 || level >= MAX_SSTABLE_LEVELS) {
        debug("Invalid SSTable level: %d", level);
        return;
    }

    LevelIndex *level_index = &_fast_access_sstables[level];

    if (level_index->count >= level_index->capacity) {
        int new_capacity = (level_index->capacity == 0) ? 1 : level_index->capacity * 2;
        SSTable **new_tables = realloc(level_index->tables, new_capacity * sizeof(SSTable *));
        if (!new_tables) {
            debug("Failed to reallocate memory for LevelIndex tables.");
            return;
        }
        level_index->tables = new_tables;
        level_index->capacity = new_capacity;
    }

    level_index->tables[level_index->count++] = sstable;
}


SSTable *_create_sstable_in_memory(char *min_key, char *max_key, int level) {
    SSTable *sstable = malloc(sizeof(SSTable));
    if (!sstable) {
        debug("Failed to allocate memory for SSTable.");
        return NULL;
    }

    char *filepath = _generate_sstable_filepath(level);
    if (!filepath) {
        free(sstable);
        debug("Failed to generate filepath for SSTable.");
        return NULL;
    }

    sstable->path = filepath;
    sstable->min_key = min_key;
    sstable->max_key = max_key;
    sstable->level = level;


    info("sstable created on memory: %s", sstable->path);
    return sstable;
}


void _write_node_to_sstable(FILE *file, AVLNode *node) {
    if (!file || !node) return;

    int key_length = strlen(node->key);
    int value_length = strlen(node->value);

    // Write the length of the key, the key itself, the length of the value, and the value
    fwrite(&key_length, sizeof(int), 1, file);
    fwrite(node->key, sizeof(char), key_length, file);

    fwrite(&value_length, sizeof(int), 1, file);
    fwrite(node->value, sizeof(char), value_length, file);
}


SSTable *_create_memtable_on_disk(Memtable *memtable, SSTable *sstable) {
    if (!memtable || !memtable->root) {
        debug("Memtable is empty or NULL.");
        return NULL;
    }
    char *min_key = _get_memtable_min_key(memtable);
    char *max_key = _get_memtable_max_key(memtable);
    FILE *file = _open_sstable(sstable, "wb");
    if (!file) {
        free(sstable->path);
        free(sstable);
        return NULL;
    }

    int min_key_length = strlen(min_key);
    int max_key_length = strlen(max_key);

    //Writes a header to the SSTable file with the min and max keys for optimal search
    fwrite(&min_key_length, sizeof(int), 1, file);
    fwrite(min_key, sizeof(char), min_key_length, file);
    fwrite(&max_key_length, sizeof(int), 1, file);
    fwrite(max_key, sizeof(char), max_key_length, file);

    memtable_traverse_in_order(memtable->root, _write_node_to_sstable);
    fclose(file);
}

// INTERFACE FUNCTIONS =============================
SSTable *dump_memtable_to_disk(Memtable *memtable, int level) {
    if (!memtable || !memtable->root) {
        debug("Memtable is empty or NULL.");
        return NULL;
    }

    char *min_key = _get_memtable_min_key(memtable);
    char *max_key = _get_memtable_max_key(memtable);

    SSTable *sstable = _create_sstable_in_memory(min_key, max_key, level);
    if (!sstable) {
        debug("Failed to create SSTable in memory.");
        return NULL;
    }

    if (_create_memtable_on_disk(memtable, sstable) == NULL) {
        free(sstable->path);
        free(sstable);
        return NULL;
    }

    info("Memtable dumped to disk as SSTable: %s", sstable->path);
    return sstable;
}