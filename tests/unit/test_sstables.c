#include "sstables.h"
#include "logging.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

int test_create_sstable_success() {
    sstable_t *sstable = create_sstable(10, 50, 0);

    ASSERT_TEST(sstable != NULL, "A struct retornou NULL");
    ASSERT_TEST(sstable->min_key == 10, "min_key atribuída incorretamente");
    ASSERT_TEST(sstable->level == 0, "Nível (level) atribuído incorretamente");

    FILE *fp = fopen(sstable->path, "rb");
    ASSERT_TEST(fp != NULL, "O arquivo físico não foi criado no disco");
    if (fp) fclose(fp);

    free(sstable->path);
    free(sstable);

    return 0;
}