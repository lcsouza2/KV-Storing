#include "sstables.h"
#include "logging.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_flush_memtable_to_disk_success() {
    Memtable *memtable = create_memtable();
    insert_memtable(memtable, "key1", "value1");
    insert_memtable(memtable, "key2", "value2");
    insert_memtable(memtable, "key3", "value3");

    SSTable *sstable = flush_memtable_to_disk(memtable, 0);

    ASSERT_TEST(sstable != NULL, "Failed to flush memtable to disk.");
    ASSERT_TEST(sstable->level == 0, "SSTable level should be 0.");
    ASSERT_TEST(strcmp(sstable->min_key, "key1") == 0, "SSTable min_key should be 'key1'.");
    ASSERT_TEST(strcmp(sstable->max_key, "key3") == 0, "SSTable max_key should be 'key3'.");

    // Clean up
    free(sstable->path);
    free(sstable);
    free_memtable(memtable);

    info("test_flush_memtable_to_disk_success passed.");
    return 0;
}