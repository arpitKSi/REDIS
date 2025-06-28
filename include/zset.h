// zset.h
#pragma once

#include "avl.h"
#include "hashtable.h"

/*
 * ZSet - Sorted Set Structure
 * ---------------------------
 * This structure combines two data structures to support:
 *   - O(log N) insertion and ordering (via AVL tree)
 *   - O(1) lookup by name (via hash table)
 *
 * Fields:
 *   - root : AVL tree root, ordered by (score, name)
 *   - hmap : Hash table for direct lookup by name
 */
struct ZSet {
    AVLNode *root = nullptr;
    HMap hmap;
};

/*
 * ZNode - Node in a ZSet
 * ----------------------
 * Each entry in the sorted set is represented by this structure.
 *
 * Fields:
 *   - tree  : AVLNode used for AVL tree ordering
 *   - hmap  : HNode used for hash table lookup
 *   - score : Value used for sorting (e.g., ranking or weight)
 *   - len   : Length of the name string
 *   - name  : Flexible array member to store the name (not null-terminated necessarily)
 */
struct ZNode {
    AVLNode tree;
    HNode   hmap;
    double  score = 0;
    size_t  len = 0;
    char    name[0];  // Flexible array to hold the name
};

/*
 * ZSet API
 * --------
 * High-level functions to manage and interact with the sorted set.
 */

// Inserts or updates a node by name with the given score
bool zset_insert(ZSet *zset, const char *name, size_t len, double score);

// Looks up a node by name
ZNode *zset_lookup(ZSet *zset, const char *name, size_t len);

// Deletes a specific node from the ZSet
void zset_delete(ZSet *zset, ZNode *node);

// Finds the first node with score >= given score (break ties with name)
ZNode *zset_seekge(ZSet *zset, double score, const char *name, size_t len);

// Clears the entire sorted set
void zset_clear(ZSet *zset);

// Returns a node that is `offset` positions ahead/behind in sorted order
ZNode *znode_offset(ZNode *node, int64_t offset);
