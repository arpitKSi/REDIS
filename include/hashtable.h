// hashtable.h
#pragma once

#include <stddef.h>
#include <stdint.h>

/*
 * HNode - Hash Node
 * -----------------
 * This is the basic unit stored in each hashtable slot.
 * It must be embedded inside the actual key/value struct (intrusive design).
 *
 * Fields:
 *   - next  : Linked list next pointer (for chaining)
 *   - hcode : Precomputed hash code for fast comparison
 */
struct HNode {
    HNode *next = nullptr;
    uint64_t hcode = 0;
};

/*
 * HTab - Simple Hash Table
 * ------------------------
 * Represents a single hashtable instance with open chaining (linked list buckets).
 * Intended for use inside HMap to support incremental rehashing.
 *
 * Fields:
 *   - tab  : Pointer to the array of slot pointers
 *   - mask : Size of the table minus one (2^n - 1) for bit-masking hash codes
 *   - size : Number of elements stored in this table
 */
struct HTab {
    HNode **tab = nullptr;
    size_t mask = 0;
    size_t size = 0;
};

/*
 * HMap - High-Level Hash Map
 * --------------------------
 * A progressive-rehashing hashtable that contains two internal tables:
 *   - newer: the table we're gradually migrating into
 *   - older: the table we're migrating out of
 *
 * This allows gradual resizing without long pauses.
 *
 * Fields:
 *   - newer        : New table that we're growing into
 *   - older        : Old table we're gradually phasing out
 *   - migrate_pos  : Current position in the migration process
 */
struct HMap {
    HTab newer;
    HTab older;
    size_t migrate_pos = 0;
};

/*
 * HMap API
 * --------
 * These functions implement the core operations on a hash map:
 *   - Lookup
 *   - Insertion
 *   - Deletion
 *   - Clearing
 *   - Iteration
 */

// Looks up a key in the map using a custom equality function
HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));

// Inserts a new node into the map
void hm_insert(HMap *hmap, HNode *node);

// Deletes a node from the map using a custom equality function
HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));

// Clears the map and frees memory
void hm_clear(HMap *hmap);

// Returns the number of items in the map
size_t hm_size(HMap *hmap);

// Iterates through all nodes, calling `f` on each. Stops early if `f` returns false.
void hm_foreach(HMap *hmap, bool (*f)(HNode *, void *), void *arg);
