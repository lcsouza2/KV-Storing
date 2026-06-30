#ifndef SETTINGS_H
#define SETTINGS_H
int get_memtable_size();
#endif
#include <stdlib.h>

#define MAX_SSTABLE_LEVEL_FILES 5
#define MAX_SSTABLE_LEVELS 7
#define MAX_MEMTABLE_SIZE get_memtable_size() // 5MB || defined by environment variable MEMTABLE_SIZE
#define SSTABLE_MAX_PATH_LENGTH 256

#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 1024

void get_sstable_path(char *buffer, size_t buffer_size);
void get_wal_path(char *buffer, size_t buffer_size);
int create_data_storage_directory();
