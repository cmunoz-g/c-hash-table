#include "hash_table.h"

// functions to allocate new hash tables, items & resizing

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

ht *ht_new_sized_ht(const size_t base_size) {
    ht *new_ht = malloc(sizeof(ht));
    if (!new_ht) {
        fprintf(stderr, "Error: malloc");
        exit(EXIT_FAILURE);
    }
    new_ht->base_size = base_size;
    new_ht->size = next_prime(new_ht->base_size);
    new_ht->count = 0;
    new_ht->items = calloc(new_ht->size, sizeof(ht_item*));
    if (!new_ht->items) {
        fprintf(stderr, "Error: calloc");
        exit(EXIT_FAILURE);
    }
    return new_ht;
}

ht *ht_new_ht() {
    return ht_new_sized_ht(HT_DEFAULT_BASE_SIZE);
}

void ht_resize_ht(ht *h, const size_t new_base_size) {
    if (new_base_size < HT_DEFAULT_BASE_SIZE) return;

    ht *new_ht = ht_new_sized_ht(new_base_size);
    for (size_t i = 0; i < h->size; i++) {
        ht_item *item = h->items[i];
        if (item && item != &HT_DELETED_ITEM)
            ht_insert(new_ht, item->key, item->value);
    }

    h->base_size = new_ht->base_size;
    h->count = new_ht->count;

    const size_t size_tmp = h->size;
    h->size = new_ht->size;
    new_ht->size = size_tmp;

    ht_item **items_tmp = h->items;
    h->items = new_ht->items;
    new_ht->items = items_tmp;

    ht_delete_ht(new_ht);
}

static void ht_resize_up(ht *h) {
    const size_t new_size = h->size * 2;
    ht_resize_ht(h, new_size);
}

static void ht_resize_down(ht *h) {
    const size_t new_size = h->size / 2;
    ht_resize_ht(h, new_size);
}

// deleting functions

void ht_delete_ht_item(ht_item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

void ht_delete_ht(ht *h) {
    for (size_t i = 0; i < h->size; i++) {
        ht_item *item = h->items[i];
        if (item && item != &HT_DELETED_ITEM)
            ht_delete_ht_item(item);
    }
    free(h->items);
    free(h);
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

void ht_insert(ht *h, const char* key, const char* value) {
    const size_t load = h->count * 100 / h->size;
    if (load > 70) ht_resize_up(h); // resizing the ht to avoid the chance of collisions

    ht_item *new_item = ht_new_item(key, value);
    size_t hashed_index = ht_get_hash(key, h->size, 0);
    ht_item *curr_item = h->items[hashed_index];
    size_t att = 1;
    
    while (curr_item && curr_item != &HT_DELETED_ITEM) {
        if (strcmp(curr_item->key, key) == 0) {
            ht_delete_ht_item(curr_item);
            h->items[hashed_index] = new_item;
            return;
        }
        hashed_index = ht_get_hash(key, h->size, att);
        curr_item = h->items[hashed_index];
        att++;
    }
        
    h->items[hashed_index] = new_item; 
    h->count++;
}

char *ht_search(ht *h, const char *key) {
    size_t hashed_index = ht_get_hash(key, h->size, 0);
    ht_item *curr_item = h->items[hashed_index];
    size_t att = 1;

    while (curr_item) {
        if (curr_item != &HT_DELETED_ITEM && strcmp(key, curr_item->key) == 0)
            return curr_item->value;
        hashed_index = ht_get_hash(key, h->size, att);
        curr_item = h->items[hashed_index];
        att++;
    }

    return NULL;
}

void ht_delete(ht *h, const char *key) {
    const size_t load = h->count * 100 / h->size;
    if (load < 10) ht_resize_down(h); // resizing the ht to avoid wasting memory

    size_t hashed_index = ht_get_hash(key, h->size, 0);
    ht_item *curr_item = h->items[hashed_index];
    size_t att = 1;

    while (curr_item) {
        if (curr_item != &HT_DELETED_ITEM && strcmp(key, curr_item->key) == 0) {
            ht_delete_ht_item(curr_item);
            h->items[hashed_index] = &HT_DELETED_ITEM;
            h->count--;
        }
        hashed_index = ht_get_hash(key, h->size, att);
        curr_item = h->items[hashed_index];
        att++;
    }
}

// int main() {
//     ht *ht = ht_new_ht();
    
//     ht_insert(ht, "arda", "guler");
//     char *searched = ht_search(ht, "arda");

//     printf("%s\n", searched);
    
//     ht_insert(ht, "arda", "not guler");
//     searched = ht_search(ht, "arda");

//     printf("%s\n", searched);

//     ht_delete_ht(ht);

//     return 0;
// }