// avl.h
#pragma once

#include <stddef.h>
#include <stdint.h>

/*
 * AVL Tree Node Structure
 * -----------------------
 * Represents a single node in an AVL (self-balancing binary search) tree.
 * Includes links to parent and children, height, and subtree node count.
 */
struct AVLNode {
    AVLNode *parent = nullptr;  // Pointer to parent node
    AVLNode *left = nullptr;    // Pointer to left child
    AVLNode *right = nullptr;   // Pointer to right child
    uint32_t height = 0;        // Height of the subtree rooted at this node
    uint32_t cnt = 0;           // Size (number of nodes) of the subtree rooted at this node
};

/*
 * Initialize a new AVL node
 * -------------------------
 * Resets the parent/children pointers and sets height & count to 1.
 */
inline void avl_init(AVLNode *node) {
    node->left = node->right = node->parent = nullptr;
    node->height = 1;
    node->cnt = 1;
}

/*
 * Helper Functions
 * ----------------
 * Returns the height or size of the subtree rooted at the given node.
 * If the node is null, it returns 0.
 */
inline uint32_t avl_height(AVLNode *node) {
    return node ? node->height : 0;
}

inline uint32_t avl_cnt(AVLNode *node) {
    return node ? node->cnt : 0;
}

/*
 * AVL Tree API Functions (Definitions in .cpp)
 * --------------------------------------------
 * These are declared here and will be implemented in the corresponding .cpp file.
 */

// Recalculates height and size of the node's subtree
AVLNode *avl_fix(AVLNode *node);

// Deletes a node from the AVL tree
AVLNode *avl_del(AVLNode *node);

// Returns the node at a specific offset from the current one (e.g., k-th smallest)
AVLNode *avl_offset(AVLNode *node, int64_t offset);
