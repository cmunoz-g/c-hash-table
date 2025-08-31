#include "hash_table.h"
#include "prime.h"
#include "xmem.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*  Constants   */
#define HT_PRIME_A 131
#define HT_PRIME_B 257
#define HT_DEFAULT_BASE_SIZE 53
#define HT_MAX_LOAD 70
#define HT_MIN_LOAD 10

/*  Internal types  */
typedef struct {
    char *key;
    char *value;
} ht_item;

struct ht {
    ht_item **items;
    size_t base_size;
    size_t size; // Bucket count
    size_t count; // Live items
    size_t tombstones; // Tracks deleted items to trigger a rehash
};

/* Tombstone sentinel. In order to preserve the collision chain, deleted items are set to point to a sentinel item  */ 
static ht_item HT_DELETED_ITEM = {NULL, NULL};

/*  -------------------
    Allocators, helpers
    -------------------
*/

static ht_item *ht_item_new(const char *k, const char* v) {
    ht_item *item = (ht_item*)xmalloc(sizeof(ht_item));
    item->key = xstrdup(k);
    item->value = xstrdup(v);
    return item;
}

static void ht_free_item(ht_item *item) {
    if (item && item != &HT_DELETED_ITEM) {
        free(item->key);
        free(item->value);
        free(item);
    }
}

static ht *ht_create_sized(size_t base_size) {
    ht *h = (ht*)xmalloc(sizeof(ht));
    if (base_size < HT_DEFAULT_BASE_SIZE) base_size = HT_DEFAULT_BASE_SIZE;
    
    h->base_size = base_size;
    h->size = next_prime(h->base_size);
    h->count = 0;

    h->items = xcalloc(h->size, sizeof(ht_item*));
    return h;
}

/*  Hash fucntion */
static size_t ht_hash(const char *s, const size_t a, const size_t num_buckets) {
    size_t hash = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        hash = (hash * a + (unsigned char)s[i]) % num_buckets;
    }
    return hash;
}

/*  Collision handler (open addressing with double hashing)  */
static size_t ht_gen_hash(const char *s, const size_t num_buckets, const size_t attempt) {
    const size_t hash_a = ht_hash(s, HT_PRIME_A, num_buckets);
    const size_t hash_b = ht_hash(s, HT_PRIME_B, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_resize(ht *h, const size_t new_base_size) {
    if (new_base_size < HT_DEFAULT_BASE_SIZE) return;

    
    ht *nh = ht_create_sized(new_base_size);
    for (size_t i = 0; i < h->size; i++) {
        ht_item *item = h->items[i];
        if (item && item != &HT_DELETED_ITEM)
        ht_set(nh, item->key, item->value);
    }
    
    h->base_size = nh->base_size;
    h->count = nh->count;
    h->tombstones = 0;
    
    const size_t size_tmp = h->size;    h->size = nh->size;     nh->size = size_tmp;
    ht_item **items_tmp = h->items;     h->items = nh->items;   nh->items = items_tmp;
    
    ht_destroy(nh);
}

static void ht_resize_up(ht *h) { ht_resize(h, h->base_size * 2); }
static void ht_resize_down(ht *h) { ht_resize(h, h->base_size / 2); }

/*  ----------
    Public API
    ----------
*/

ht *ht_create() {
    return ht_create_sized(HT_DEFAULT_BASE_SIZE);
}

ht *ht_create_with_size(const size_t base_size) {
    return ht_create_sized(base_size ? base_size : HT_DEFAULT_BASE_SIZE);
}

void ht_destroy(ht *h) {
    if (!h) return;
    for (size_t i = 0; i < h->size; i++) {
        ht_item *item = h->items[i];
        if (item && item != &HT_DELETED_ITEM)
            ht_free_item(item);
    }
    free(h->items);
    free(h);
}

bool ht_set(ht *h, const char* key, const char* value) {
    const size_t load = h->count * 100 / h->size;
    if (load > HT_MAX_LOAD) ht_resize_up(h);

    size_t idx = ht_gen_hash(key, h->size, 0);
    ht_item *cur = h->items[idx];
    size_t att = 1;
    
    while (cur && cur != &HT_DELETED_ITEM) {
        if (strcmp(cur->key, key) == 0) {
            char *new_val = xstrdup(value);
            free(cur->value);
            cur->value = new_val;
            return false;
        }
        idx = ht_gen_hash(key, h->size, att++);
        cur = h->items[idx];
        if (att >= h->size) {
            fprintf(stderr, "overflow\n");
            exit(0);
        }
    }
        
    h->items[idx] = ht_item_new(key, value);
    h->count++;
    return true;
}

const char *ht_get(const ht *h, const char *key) {
    size_t idx = ht_gen_hash(key, h->size, 0);
    ht_item *cur = h->items[idx];
    size_t att = 1;

    while (cur) {
        if (cur != &HT_DELETED_ITEM && strcmp(key, cur->key) == 0)
            return cur->value;
        idx = ht_gen_hash(key, h->size, att++);
        cur = h->items[idx];
    }
    return NULL;
}

bool ht_remove(ht *h, const char *key) {
    size_t idx = ht_gen_hash(key, h->size, 0);
    ht_item *cur = h->items[idx];
    size_t att = 1;

    while (cur) {
        if (cur != &HT_DELETED_ITEM && strcmp(key, cur->key) == 0) {
            ht_free_item(cur);
            h->items[idx] = &HT_DELETED_ITEM;
            h->count--;
            
            const size_t load = h->count * 100 / h->size;
            if (load < HT_MIN_LOAD) ht_resize_down(h); 

            if (h->tombstones > h->size / 5) 
                ht_resize(h, h->base_size);    

            return true;
        }
        idx = ht_gen_hash(key, h->size, att++);
        cur = h->items[idx];
    }
    return false;    
}

size_t ht_count(const ht *h) { return h ? h->count : 0; }

size_t ht_size(const ht *h) { return h ? h->size : 0; }

size_t ht_load_factor(const ht *h) { return (h && h->size) ? (double)h->count / (double)h->size : 0.0; }