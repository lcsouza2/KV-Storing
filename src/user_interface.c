#include <stdio.h>
#include "memtable.h"
#include "logging.h"
#include "settings.h"
#include "wal.h"
#include "user_interface.h"
#include "sstables.h"

void db_insert(Database *db, const char *key, const char *value) {
    append_wal_log(key, value);

    Memtable *tree = insert_memtable(db->memtable, (char *)key, (char *)value);
    db->memtable = tree;

    if (value) info("Client INSERT: %s -> %s", key, value);
    else info("Client DELETE: %s", key);

    if (tree->bytes_allocated > MAX_MEMTABLE_SIZE) {
        info("Memtable threshold exceeded. Flushing...");
        if (flush_memtable_to_disk(tree, 0) == 0) {
            clear_memtable(tree);
            clear_wal_logs();
        }
    }
}

void init_database(Database *db) {
    db->memtable = create_memtable();
    if (!db->memtable) {
        error("Failed to initialize memtable.");
        return;
    }
    sync_wal_to_memtable(db->memtable);
}


void clear() {
    printf("\033[H\033[J");
}

void print_main_menu() {
    printf("=== Main Menu ===\n");
    printf("1. Add Key-Value Pair\n");
    printf("2. Get Value by Key\n");
    printf("3. Update Value by Key\n");
    printf("4. Delete Key-Value Pair\n");
    printf("5. Display All Key-Value Pairs\n");
    printf("6. Exit\n");
    printf("Choose an option: ");
}


int main() {
    Database *db = malloc(sizeof(Database));
    init_database(db);

    while (1) {
        print_main_menu();
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                char key[MAX_KEY_LENGTH];
                char value[1];
                db_put(db, key, value);
                break;
            case 2:
                // Get Value by Key
                break;
            case 3:
                // Update Value by Key
                break;
            case 4:
                // Delete Key-Value Pair
                break;
            case 5:
                // Display All Key-Value Pairs
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}