#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include "memtable.h"
typedef struct key_value {
    char *key;
    char *value;
} KeyValue;

typedef struct database {
    Memtable *memtable;
} Database;

void db_insert(Database *db, const char *key, const char *value);
#endif
