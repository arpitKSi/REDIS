// hashtable.cpp
#include <assert.h>
#include <stdlib.h>     // calloc(), free()
#include "hashtable.h"

/**
 * h_init - Initialize a hash table with specified capacity
 * @htab: Hash table to initialize
 * @n: Capacity (must be a power of 2)
 * 
 * Sets up a hash table with n buckets. The capacity must be a power of 2
 * to allow efficient bit-masking for hash code modulo operations.
 * 
 * Preconditions: n > 0 and n is a power of 2
 */
static void h_init(HTab *htab, size_t n) {
    assert(n > 0 && ((n - 1) & n) == 0);  // check if n is power of 2
    htab->tab = (HNode **)calloc(n, sizeof(HNode *)); // allocate empty bucket array
    htab->mask = n - 1;                  // used for efficient modulo (hcode & mask)
    htab->size = 0;
}

/**
 * h_insert - Insert a node into the hash table
 * @htab: Hash table to insert into
 * @node: Node to insert (must have hcode field set)
 * 
 * Inserts the node at the front of its bucket's chain (LIFO order).
 * The hash code must be precomputed and stored in node->hcode.
 */
static void h_insert(HTab *htab, HNode *node) {
    size_t pos = node->hcode & htab->mask;
    HNode *next = htab->tab[pos];
    node->next = next;
    htab->tab[pos] = node;
    htab->size++;
}

/**
 * h_lookup - Find pointer to pointer to a node matching the key
 * @htab: Hash table to search in
 * @key: Key node to match against
 * @eq: Equality comparison function
 * 
 * Searches for a node equivalent to 'key' using the provided equality function.
 * Returns a pointer to the pointer that points to the found node, which is
 * useful for deletion (allows modification of the previous node's next pointer).
 * 
 * Returns: Pointer to the 'next' field that points to the matching node,
 *          or NULL if not found
 */
static HNode **h_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *)) {
    if (!htab->tab) return NULL;

    size_t pos = key->hcode & htab->mask;
    HNode **from = &htab->tab[pos];

    for (HNode *cur; (cur = *from) != NULL; from = &cur->next) {
        if (cur->hcode == key->hcode && eq(cur, key)) {
            return from;
        }
    }
    return NULL;
}

// Remove a node from the hash table chain, given a pointer to its reference
static HNode *h_detach(HTab *htab, HNode **from) {
    HNode *node = *from;
    *from = node->next;  // remove node from the linked list
    htab->size--;
    return node;
}

// How much rehashing to do incrementally each time
const size_t k_rehashing_work = 128;

// Migrate up to `k_rehashing_work` nodes from the old table to the new one
static void hm_help_rehashing(HMap *hmap) {
    size_t nwork = 0;
    while (nwork < k_rehashing_work && hmap->older.size > 0) {
        HNode **from = &hmap->older.tab[hmap->migrate_pos];
        if (!*from) {
            hmap->migrate_pos++;
            continue; // skip empty slots
        }
        // Move one node to newer table
        h_insert(&hmap->newer, h_detach(&hmap->older, from));
        nwork++;
    }

    // Cleanup old table once it's empty
    if (hmap->older.size == 0 && hmap->older.tab) {
        free(hmap->older.tab);
        hmap->older = HTab{}; // reset
    }
}

// Begin rehashing: double table size and move old table to `older`
static void hm_trigger_rehashing(HMap *hmap) {
    assert(hmap->older.tab == NULL); // should not already be rehashing
    hmap->older = hmap->newer;       // move current table to `older`
    h_init(&hmap->newer, (hmap->newer.mask + 1) * 2); // allocate bigger table
    hmap->migrate_pos = 0;
}

// Public: Look up a key from either new or old table (depending on migration)
HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
    hm_help_rehashing(hmap); // perform partial rehash
    HNode **from = h_lookup(&hmap->newer, key, eq);
    if (!from) {
        from = h_lookup(&hmap->older, key, eq);
    }
    return from ? *from : NULL;
}

// Max load factor before rehashing
const size_t k_max_load_factor = 8;

// Public: Insert a node into the map
void hm_insert(HMap *hmap, HNode *node) {
    if (!hmap->newer.tab) {
        h_init(&hmap->newer, 4); // initialize table if it's not created yet
    }

    h_insert(&hmap->newer, node); // always insert into the newer table

    // Check if load factor exceeded — trigger rehashing
    if (!hmap->older.tab) {
        size_t threshold = (hmap->newer.mask + 1) * k_max_load_factor;
        if (hmap->newer.size >= threshold) {
            hm_trigger_rehashing(hmap);
        }
    }

    hm_help_rehashing(hmap); // perform incremental migration
}

// Public: Delete a node from the map and return it
HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
    hm_help_rehashing(hmap);
    if (HNode **from = h_lookup(&hmap->newer, key, eq)) {
        return h_detach(&hmap->newer, from);
    }
    if (HNode **from = h_lookup(&hmap->older, key, eq)) {
        return h_detach(&hmap->older, from);
    }
    return NULL;
}

// Public: Deallocate and reset the map
void hm_clear(HMap *hmap) {
    free(hmap->newer.tab);
    free(hmap->older.tab);
    *hmap = HMap{}; // reset all fields to zero/null
}

// Public: Return total number of elements
size_t hm_size(HMap *hmap) {
    return hmap->newer.size + hmap->older.size;
}

// Internal: Apply function `f` to all nodes in a single table
static bool h_foreach(HTab *htab, bool (*f)(HNode *, void *), void *arg) {
    for (size_t i = 0; htab->mask != 0 && i <= htab->mask; i++) {
        for (HNode *node = htab->tab[i]; node != NULL; node = node->next) {
            if (!f(node, arg)) {
                return false; // early exit if callback fails
            }
        }
    }
    return true;
}

// Public: Apply function `f` to all nodes in both newer and older tables
void hm_foreach(HMap *hmap, bool (*f)(HNode *, void *), void *arg) {
    h_foreach(&hmap->newer, f, arg) && h_foreach(&hmap->older, f, arg);
}
