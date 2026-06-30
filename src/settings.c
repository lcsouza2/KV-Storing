#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include "logging.h"

static char *_get_data_storage_path() {
    char *path = getenv("DATA_PATH");
    if (path == NULL) {
        return "./data";
    }
    return path;
}

void get_sstable_path(char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return;
    const char *base_path = _get_data_storage_path();
    snprintf(buffer, buffer_size, "%s/sstables", base_path);
}

void get_wal_path(char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return;
    const char *base_path = _get_data_storage_path();
    snprintf(buffer, buffer_size, "%s/wal", base_path);
}

int create_data_storage_directory() {
    char sstables_path[256];
    char wal_path[256];
    char *data_path = _get_data_storage_path();
    get_sstable_path(sstables_path, sizeof(sstables_path));
    get_wal_path(wal_path, sizeof(wal_path));

    if (mkdir(data_path, 0755) == -1 && errno != EEXIST) {
        error("Failed to create data directory");
        return -1;
    }

    // Create the sstables directory inside the data directory
    if (mkdir(sstables_path, 0755) == -1 && errno != EEXIST) {
        error("Failed to create sstables directory");
        return -1;
    }

    // Create the wal directory inside the data directory
    if (mkdir(wal_path, 0755) == -1 && errno != EEXIST) {
        error("Failed to create wal directory");
        return -1;
    }

    return 0;
}