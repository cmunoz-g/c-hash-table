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

ht *ht_new_ht() {
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

void ht_delete_ht_item(ht_item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

void ht_delete_ht(ht *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item && item != &HT_DELETED_ITEM)
            ht_delete_ht_item(item);
    }
    free(ht->items);
    free(ht);
}

// hash function

static size_t ht_hash(const char *s, const size_t a, const size_t num_buckets) {
    long hash = 0;
    const size_t s_len = strlen(s);
    for (size_t i = 0; i < s_len; i++) {
        hash = (hash * a + s[i]) % num_buckets;
    }
    return (size_t)hash;
}

// collision handler (open addressing with double hashing)

static size_t ht_get_hash(const char *s, const size_t num_buckets, const size_t attempt) {
    const size_t first_hash = ht_hash(s, HT_PRIME_1, num_buckets);
    const size_t second_hash = ht_hash(s, HT_PRIME_2, num_buckets);
    return (first_hash + (attempt * (second_hash + 1))) % num_buckets;
}

// api

void ht_insert(ht *table, const char* key, const char* value) {
    ht_item *new_item = ht_new_item(key, value);
    size_t hashed_index = ht_get_hash(key, table->size, 0);
    ht_item *curr_item = table->items[hashed_index];
    size_t att = 1;
    
    while (curr_item) {
        hashed_index = ht_get_hash(key, table->size, att);
        curr_item = table->items[hashed_index];
        att++;
    }
        
    table->items[hashed_index] = new_item; 
    table->count++;
}

char *ht_search(ht *table, const char *key) {
    size_t hashed_index = ht_get_hash(key, table->size, 0);
    ht_item *curr_item = table->items[hashed_index];
    size_t att = 1;

    while (curr_item) {
        if (curr_item != &HT_DELETED_ITEM && strcmp(key, curr_item->key) == 0) return curr_item->value;
        hashed_index = ht_get_hash(key, table->size, att);
        curr_item = table->items[hashed_index];
        att++;
    }

    return NULL;
}

void ht_delete(ht *table, const char *key) {
    size_t hashed_index = ht_get_hash(key, table->size, 0);
    ht_item *curr_item = table->items[hashed_index];
    size_t att = 1;

    while (curr_item) {
        if (curr_item != &HT_DELETED_ITEM && strcmp(key, curr_item->key) == 0) {
            ht_delete_ht_item(curr_item);
            table->items[hashed_index] = &HT_DELETED_ITEM;
            table->count--;
        }
        hashed_index = ht_get_hash(key, table->size, att);
        curr_item = table->items[hashed_index];
        att++;
    }
}

// int main() {
//     ht *ht = ht_new_ht();
    
//     ht_insert(ht, "arda", "guler");
//     char *searched = ht_search(ht, "arda");

//     printf("before deleting %s\n", searched);

//     ht_delete(ht, "arda");

//     char *deleted = ht_search(ht, "arda");
//     if (!deleted) printf("arda was deleted\n");
//     else printf("something went wrong\n");

//     ht_delete_ht(ht);

//     return 0;
// }