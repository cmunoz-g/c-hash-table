#include "test.h"
#include "hash_table.h"

#define HT_RESIZE_N 5000

void test_ht_crud() {
    ht *h = ht_create();

    ASSERT_TRUE("missing key returns NULL", ht_get(h, "key") == NULL);
    ASSERT_TRUE("ht_set() returns true on insertion", ht_set(h, "key", "value") == true);
    ASSERT_TRUE("ht_get() returns the correct value", strcmp(ht_get(h, "key"), "value") == 0);
    ASSERT_TRUE("ht_remove() removes an existing item", ht_remove(h, "key") == true);
    ASSERT_TRUE("removed item can no longer be found", ht_get(h, "key") == NULL);
    ASSERT_TRUE("ht_remove() on a non-existing key returns false", ht_remove(h, "key") == false);
    ASSERT_TRUE("ht_set() can reinsert after deletion", ht_set(h, "key", "new_value"));
    ASSERT_TRUE("ht_get() returns NULL on missing keys in the presence of tombstones", ht_get(h, "no_key") == NULL);
    ASSERT_TRUE("ht_get() retrieves existing item in the presence of tombstones", strcmp(ht_get(h, "key"), "new_value") == 0);

    ht_destroy(h);
}

void test_ht_overwrite() {
    ht *h = ht_create();

    ht_set(h, "key", "value");
    size_t count_after_first_set = ht_count(h);
    
    ASSERT_TRUE("ht_set() returns false when overwrites", ht_set(h, "key", "new_value") == false);
    ASSERT_TRUE("ht_set() correctly updates the value of an existing key", strcmp(ht_get(h, "key"), "new_value") == 0);
    ASSERT_TRUE("ht_count stays the same after overwritting", ht_count(h) == count_after_first_set);

    ht_destroy(h);
}

void test_ht_collisions() {
    ht *h = ht_create();

    ht_set(h, "aababa", "first_value");
    ASSERT_TRUE("ht_set() inserts correctly when there is a collision", ht_set(h, "abaaaa", "second_value") == true);
    ASSERT_TRUE("first value inserted can be retrieved", strcmp(ht_get(h, "aababa"), "first_value") == 0);
    ASSERT_TRUE("second value inserted can be retrieved", strcmp(ht_get(h, "abaaaa"), "second_value") == 0);

    ht_destroy(h);
}

void test_ht_resize() {
    ht *h = ht_create();

    char key[64];
    for (size_t i = 0; i < HT_RESIZE_N; i++) {
        snprintf(key, sizeof(key), "k_%zu", i);
        ASSERT_TRUE("ht_set() inserts all keys", ht_set(h, key, "value") == true);
    }

    // for (size_t i = 0; i < HT_RESIZE_N; i++) {
    //     snprintf(key, sizeof(key), "k_%zu", i);
    //     ASSERT_TRUE("ht_get() retrieves all keys", strcmp(ht_get(h, key), "value") == 0);
    // }

    // for (size_t i = 0; i < HT_RESIZE_N; i++) {
    //     if (i % 2 == 0) {
    //         snprintf(key, sizeof(key), "k_%zu", i);
    //         ASSERT_TRUE("removal of keys", ht_remove(h, key) == true);
    //     }
    // }

    // for (size_t i = 1; i < HT_RESIZE_N; i+=2) {
    //     snprintf(key, sizeof(key), "k_%zu", i);
    //     ASSERT_TRUE("ht_get() retrieves survivors after heavy removals", strcmp(ht_get(h, key), "value") == 0);
    // }

    // for (size_t i = 0; i < HT_RESIZE_N; i +=2) {
    //     snprintf(key, sizeof(key), "k_%zu", i);
    //     ASSERT_TRUE("deleted items are missing", ht_get(h, key) == NULL);
    // }

    // for (size_t i = 0; i < HT_RESIZE_N; i += 2) {
    //     snprintf(key, sizeof key, "k_%zu", i);
    //     ASSERT_TRUE("reinsert items into tombstoned table", ht_set(h, key, "new_value") == true);
    //     ASSERT_TRUE("value of reinserted items is updated correctly", strcmp(ht_get(h, key), "new_value") == 0);
    // }

    ht_destroy(h);
}

void test_ht_edge_cases() {
    ht *h = ht_create();

    ASSERT_TRUE("ht_set() can create an empty key", ht_set(h, "", "value") == true);
    ASSERT_TRUE("ht_set() can create an empty value", ht_set(h, "key", "") == true);
    ASSERT_TRUE("ht_get() retrieves item with empty key", strcmp(ht_get(h, ""), "value") == 0);
    ASSERT_TRUE("ht_get() retrieves the correct empty value", strcmp(ht_get(h, "key"), "") == 0);
    ASSERT_TRUE("ht_set() with high-bit bytes in key works", ht_set(h, "\xFF\x00\xC3\xA9", "value") == true);
    ASSERT_TRUE("ht_get() retrieves the item with high-bit bytes in key", strcmp(ht_get(h, "\xFF\x00\xC3\xA9"), "value") == 0);

    char *m[4] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2",
                "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"};
    bool b[4] = {false, false, false, false};

    for (size_t i = 0; i < 4; i++) {
        b[i] = ht_set(h, m[i], "value");
    }

    ASSERT_TRUE("ht_set() works on keys sharing long prefixes", (b[0] && b[1] && b[2] && b[3]));
    ASSERT_TRUE("ht_get() recovers values of keys sharing long prefixes", strcmp(ht_get(h, m[2]), "value") == 0);

    ht_destroy(h);
}
