#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ht ht;

typedef enum {
    HT_OK_INSERTED = 0,
    HT_OK_REPLACED = 1,
    HT_ERR_OOM     = -1,
    HT_ERR_CAP     = -2,  // ht cannot grow any longer
} ht_result;

/*  ---------------------
    Creation, Destruction
    ---------------------
*/

ht *ht_create(void);
/*  Creates a hash table with an initial base size, rounded up to a prime number    */
ht* ht_create_with_size(size_t base_size);
void ht_destroy(ht *table);

/*  ----------------
    Basic Operations
    ----------------
*/

/*  Inserts or overwrites the value for key. Returns true for new insertions and false for updates. 
    Keys and values are copied internally.  */
bool ht_set(ht *table, const char *key, const char *value);

/*  Returns a non-owned pointer to the stored value*/
const char *ht_get(const ht *table, const char *key);

/*  Removes key, if present. Returns true if the key existed and was removed. False otherwise   */
bool ht_remove(ht *table, const char *key);

/*  Introspection */
size_t ht_count(const ht *h);
size_t ht_size(const ht *h);
double ht_load_factor(const ht *h);

#ifdef __cplusplus
}
#endif

#endif /* HASH_TABLE_H */