# c-hash-table

> Custom implementation of a hash table data structure in C; features open addressing with double hashing, prime-sized bucket arrays and automatic resizes.


## Start

### Prerequisites

* A C11-capable compiler (e.g., `gcc` or `clang`).
* `make`
* `valgrind` for leak checks in the tests.

### Build & run

The static library and binaries are written to `build/`.

```bash
# Clone:
git clone https://github.com/cmunoz-g/c-hash-table c-hash-table
cd c-hash-table

# Build:
make

# Run the example:
make run

# Run the tests:
make tests
```

### Make targets

| Target        | What it does                                                 |
| ------------- | ------------------------------------------------------------ |
| `make`        | Builds static lib `build/libhashtable.a` and example binary. |
| `make run`    | Builds (if needed) and runs `examples/basic.c`.              |
| `make tests`  | Builds & run the unit tests.                                 |
| `make fclean` | Removes build artifacts and the binary defined by `NAME`.    |


## Using as a Library

1. **Add include path** and **link the static lib**:

   ```bash
   gcc -std=c11 -Wall -Wextra -Iinclude your_app.c build/libhashtable.a -o your_app
   ```

2. **Include the header** in your source:

   ```c
   #include "hash_table.h"
   ```

> The library exposes a C API and returns non-owning `const char*` for value queries. Returned strings are owned by the table, so they must not be freed or modified.


## API Overview

| Function                                                      | Purpose                                   | Returns / Notes                                 |
| ------------------------------------------------------------- | ----------------------------------------- | ----------------------------------------------- |
| `ht *ht_create(void);`                                        | Creates a default-sized table             | Returns heap-allocated table; call `ht_destroy` |
| `ht* ht_create_with_size(size_t base_size);`                  | Creates with base size (rounded to prime) |                                                 |
| `void ht_destroy(ht *table);`                                 | Frees table and contents                  |                                                 |
| `bool ht_set(ht *table, const char *key, const char *value);` | Inserts or overwrites                     | `true` on new insert, `false` on update         |
| `const char *ht_get(const ht *table, const char *key);`       | Lookup                                    | `NULL` if missing; pointer is non-owning        |
| `bool ht_remove(ht *table, const char *key);`                 | Deletes if present                        | Tombstones; may trigger resize/rehash           |
| `size_t ht_count(const ht *h);`                               | Number of live items                      |                                                 |
| `size_t ht_size(const ht *h);`                                | Bucket array size                         |                                                 |
| `double ht_load_factor(const ht *h);`                         | `count / size`                            |                                                 |

---

## Design & Implementation Notes

* **Hashing:** polynomial rolling hash; it double hashes with distinct primes for `hash_a` and `hash_b` to reduce inputs hashing to the same output
* **Collision handling:** open addressing with step `1 + (hash_b % (buckets - 1))`
* **Resizing policy:** grows/shrinks around load thresholds (70% for resizing down, 10% for resizing up); capacity is always the next prime ≥ base size
* **Deletion strategy:** tombstones preserve probe chains; there is a periodic rehash to clean up when they occupy too much space
* **Memory model:** keys/values are copied with `xstrdup`; OOM is fatal via `xmalloc`/`xcalloc`

## Complexity

* **Average:** O(1) set/get/remove
* **Worst-case:** O(n)
* **Space:** O(n) plus tombstones
