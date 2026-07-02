#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "settings.h"
#include "sstables.h"
#include "k_way_iterator.h"
#include "logging.h"

/**
 * @brief Reads a dynamic input from standard input.
 * @return (char *): a pointer to the allocated string, NULL on failure.
 */
char *read_dynamic_input() {
    size_t capacity = 64;
    size_t length = 0;

    char *buffer = malloc(capacity);
    if (!buffer) return NULL;

    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF) {
        buffer[length++] = (char)c;

        if (length == capacity) {
            capacity *= 2;

            char *new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
    }

    buffer[length] = '\0';

    if (length == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

char *generate_sstable_filepath(int level, int id) {
    char filepath[SSTABLE_MAX_PATH_LENGTH];
    char sstable_path[SSTABLE_MAX_PATH_LENGTH / 2];
    get_sstable_path(sstable_path, sizeof(sstable_path));

    snprintf(
        filepath,
        sizeof(filepath),
        "%s/L%d_%d.dat",
        sstable_path,
        level,
        id
    );

    return strdup(filepath);
}

int clear_sstables_after_test() {
    for (int id = 0; id < MAX_SSTABLE_LEVEL_FILES; id++) {
        for (int level = 0; level < MAX_SSTABLE_LEVELS; level++) {
            delete_sstable(level, id);
        }
    }
    return 0;
}

static int _should_compact_sstables(int level) {
    int count = get_sstable_count(level);
    return count > MAX_SSTABLE_LEVEL_FILES;
}

void check_and_compact_sstables(int level) {
    if (level >= MAX_SSTABLE_LEVELS - 1) {
        return;
    }

    if (_should_compact_sstables(level)) {
        info("Compacting SSTables at level %d", level);
        compact_sstables(get_sstable_count(level), level);
        check_and_compact_sstables(level + 1);
    }

    return;
}

