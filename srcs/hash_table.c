#include "hash_table.h"

// functions to allocate new hash tables and items

static ht_item *ht_new_item(const char *k, const char* v) {
    ht_item *new_item = malloc(sizeof(ht_item));
    if (!new_item) {
        fprintf(stderr, "Error: malloc");
        exit(EXIT_FAILURE);
    }
    new_item->key = strdup(k);
    new_item->value = strdup(v);
    return new_item;
}

ht *ht_new() {
    ht *new_ht = malloc(sizeof(ht));
    if (!new_ht) {
        fprintf(stderr, "Error: malloc");
        exit(EXIT_FAILURE);
    }
    new_ht->size = 42;
    new_ht->count = 0;
    new_ht->items = calloc(new_ht->size, sizeof(ht_item*));
    if (!new_ht->items) {
        fprintf(stderr, "Error: calloc");
        exit(EXIT_FAILURE);
    }
    return new_ht;
}

// deleting functions

void delete_ht_item(ht_item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

void delete_ht(ht *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item)
            delete_ht_item(item);
    }
    free(ht->items);
    free(ht);
}

// hash function

static size_t hash(const char *s, const size_t a, const size_t num_buckets) {
    long hash = 0;
    const size_t s_len = strlen(s);
    for (size_t i = 0; i < s_len; i++) {
        hash = (hash * a + s[i]) % num_buckets;
    }
    return (size_t)hash;
}

// collision handler (open addressing with double hashing)

static size_t get_hash(const char *s, const size_t num_buckets, const size_t attempt) {
    const size_t first_hash = hash(s, HT_PRIME_1, num_buckets);
    const size_t second_hash = hash(s, HT_PRIME_2, num_buckets);
    return (first_hash + (attempt * (second_hash + 1))) % num_buckets;
}

// int main() {
//     ht *ht = ht_new();
//     ht_item *ht_i = ht_new_item("c", "v"); 
//     delete_ht(ht);
//     delete_ht_item(ht_i);

//     size_t hashed = hash("cat", 163, 53);
//     printf("%d\n", (int)hashed);
//     return 0;
// }