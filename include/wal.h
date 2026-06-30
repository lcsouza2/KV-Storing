#ifndef WAL_H
#define WAL_H

void append_wal_log(const char *key, const char *value);
int sync_wal_to_memtable(Memtable *memtable);
int clear_wal_logs();
#endif