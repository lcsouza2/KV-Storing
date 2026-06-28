#include <stdio.h>
#include "logging.h"
#include "sstables.h"

extern int test_create_sstable_success();

int main() {
    int failed = 0;

    info("Running tests...");

    if (test_create_sstable_success() != 0) failed++;
    if (failed > 0) {
        error("%d test(s) failed.", failed);
        return -1;
    }

    info("All tests passed!");
    return 0;
}