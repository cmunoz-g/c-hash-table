#include "test.h"
#include "hash_table.h"

void test_ht_crud() {
    ht *h = ht_create();

    ASSERT_TRUE("missing key returns NULL", ht_get(h, "key") == NULL);
    ASSERT_TRUE("ht_set() returns true on insertion", ht_set(h, "key", "value") == true);
    ASSERT_TRUE("ht_get() returns the correct value", strcmp(ht_get(h, "key"), "value") == 0);
    ASSERT_TRUE("ht_remove() removes an existing item", ht_remove(h, "key") == true);
    ASSERT_TRUE("removed item can no longer be found", ht_get(h, "key") == NULL);
    ASSERT_TRUE("ht_remove() on a non-existing key returns false", ht_remove(h, "key") == false);
    ASSERT_TRUE("ht_set() can reinsert after deletion", ht_set(h, "key", "value"));

    ht_destroy(h);
}

void test_ht_overwrite() {
    ht *h = ht_create();

    ht_set(h, "key", "value");
    size_t count_after_first_set = ht_count(h);
    
    ASSERT_TRUE("ht_set() returns false when overwrites", ht_set(h, "key", "new_value") == false);
    ASSERT_TRUE("ht_set() correctly updates the value of an existing key", strcmp(ht_get(h, "key"), "new_value"));
    ASSERT_TRUE("ht_count stays the same after overwritting", ht_count(h) == count_after_first_set);
}

void test_ht_collisions() {
    ht *h = ht_create();

    ht_set(h, "aababa", "first_value");
    ASSERT_TRUE("ht_set() inserts correctly when there is a collision", ht_set(h, "abaaaa", "second_value") == true);
    ASSERT_TRUE("first value inserted can be retrieved", strcmp(ht_get(h, "aababa"), "first_value"));
    ASSERT_TRUE("second value inserted can be retrieved", strcmp(ht_get(h, "aababa"), "second_value"));
}