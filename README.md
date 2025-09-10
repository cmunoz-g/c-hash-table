# #️ c-hash-table

Custom implementation of a hash table in C with open addressing, double hashing, prime-sized buckets, and automatic resizing.

## Build and run

Requires a C11 compiler (`gcc` or `clang`), `make`, and `valgrind` (for tests).
All outputs are written to `build/`.

```bash
# Clone:
git clone https://github.com/cmunoz-g/c-hash-table c-hash-table
cd c-hash-table

# Build library and example:
make

# Run the example:
make run

# Run unit tests:
make tests
```

### Make targets

| Target        | Description                                                 |
| ------------- | ----------------------------------------------------------- |
| `make`        | Builds static lib `build/libhashtable.a` and example binary |
| `make run`    | Runs `examples/basic.c`                                     |
| `make tests`  | Builds and runs the unit tests                              |
| `make fclean` | Removes all build artifacts                                 |

## Library usage

1. **Link the library**:

   ```bash
   gcc -std=c11 -Wall -Wextra -Iinclude your_app.c build/libhashtable.a -o your_app
   ```

2. **Include the header**:

   ```c
   #include "hash_table.h"
   ```

The API returns non-owning `const char *` for lookups.
These strings are owned by the table and must not be freed or modified.

## API

| Function                                                      | Purpose                            | Notes                                        |
| ------------------------------------------------------------- | ---------------------------------- | -------------------------------------------- |
| `ht *ht_create(void);`                                        | Create default-sized table         | Must free with `ht_destroy`                  |
| `ht *ht_create_with_size(size_t base_size);`                  | Create with custom base size       | Size rounded up to next prime                |
| `void ht_destroy(ht *table);`                                 | Free table and contents            |                                              |
| `bool ht_set(ht *table, const char *key, const char *value);` | Insert or update                   | Returns `true` if new, `false` if overwrite  |
| `const char *ht_get(const ht *table, const char *key);`       | Lookup                             | Returns owned pointer or `NULL` if not found |
| `bool ht_remove(ht *table, const char *key);`                 | Remove if present                  | Leaves tombstone, may trigger resize/rehash  |
| `size_t ht_count(const ht *h);`                               | Live item count                    |                                              |
| `size_t ht_size(const ht *h);`                                | Current bucket array size          |                                              |
| `double ht_load_factor(const ht *h);`                         | Current load factor (`count/size`) |                                              |

## Implementation details

* **Hashing:** polynomial rolling hash with distinct primes for `hash_a` and `hash_b`.
* **Collisions:** resolved with open addressing, step = `1 + (hash_b % (buckets - 1))`.
* **Resizing:** expands/shrinks based on load factor thresholds (grow above \~70%, shrink below \~10%). Capacity always set to a prime.
* **Deletions:** use tombstones to preserve probe chains. Periodic rehash cleans up when tombstones accumulate.
* **Memory model:** keys and values copied with `xstrdup`. Allocation failures are fatal via `xmalloc`/`xcalloc`.

## Complexity

* **Average case:** O(1) for set/get/remove
* **Worst case:** O(n)
* **Space:** O(n) plus tombstones

