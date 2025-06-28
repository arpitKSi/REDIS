// avl.cpp
#include <assert.h>
#include "avl.h"

/**
 * max - Return the maximum of two uint32_t values
 * @lhs: First value to compare
 * @rhs: Second value to compare
 * 
 * Returns: The larger of the two values
 */
static uint32_t max(uint32_t lhs, uint32_t rhs) {
    return lhs < rhs ? rhs : lhs;
}

/**
 * avl_update - Update height and subtree node count of a node 
 * @node: AVL node to update
 * 
 * Recalculates the height and count fields of the given node based on
 * its children. This must be called after any structural change to
 * maintain AVL tree invariants.
 * 
 * Height = 1 + max(left_height, right_height)
 * Count = 1 + left_count + right_count
 */
static void avl_update(AVLNode *node) {
    node->height = 1 + max(avl_height(node->left), avl_height(node->right));
    node->cnt = 1 + avl_cnt(node->left) + avl_cnt(node->right);
}

/**
 * rot_left - Perform left rotation and return new subtree root
 * @node: Root of the subtree to rotate
 * 
 * Performs a left rotation around the given node. This is used to
 * maintain AVL balance when the right subtree is too heavy.
 * 
 * Before:     node           After:      new_node
 *            /    \                     /        \
 *         left   new_node            node       right
 *               /       \            /    \
 *            inner     right      left   inner
 * 
 * Returns: The new root of the rotated subtree (new_node)
 */
static AVLNode *rot_left(AVLNode *node) {
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->right;
    AVLNode *inner = new_node->left;

    // Rewire node's right child to inner
    node->right = inner;
    if (inner) {
        inner->parent = node;
    }

    // Connect new_node to the original parent
    new_node->parent = parent;

    // new_node becomes the new root of this subtree
    new_node->left = node;
    node->parent = new_node;

    // Update heights and counts
    avl_update(node);
    avl_update(new_node);

    return new_node;
}

// Perform right rotation and return new subtree root
static AVLNode *rot_right(AVLNode *node) {
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->left;
    AVLNode *inner = new_node->right;

    // Rewire node's left child
    node->left = inner;
    if (inner) {
        inner->parent = node;
    }

    // Connect new_node to parent
    new_node->parent = parent;

    // new_node becomes the new root of this subtree
    new_node->right = node;
    node->parent = new_node;

    // Update heights and counts
    avl_update(node);
    avl_update(new_node);

    return new_node;
}

// Fix left-heavy imbalance (height difference is 2, left is taller)
static AVLNode *avl_fix_left(AVLNode *node) {
    // Perform left-right rotation if needed
    if (avl_height(node->left->left) < avl_height(node->left->right)) {
        node->left = rot_left(node->left);
    }
    return rot_right(node);
}

// Fix right-heavy imbalance (height difference is 2, right is taller)
static AVLNode *avl_fix_right(AVLNode *node) {
    // Perform right-left rotation if needed
    if (avl_height(node->right->right) < avl_height(node->right->left)) {
        node->right = rot_right(node->right);
    }
    return rot_left(node);
}

// Rebalance tree from a modified node up to the root
AVLNode *avl_fix(AVLNode *node) {
    while (true) {
        AVLNode **from = &node; // Pointer to where the current node is attached
        AVLNode *parent = node->parent;

        if (parent) {
            from = parent->left == node ? &parent->left : &parent->right;
        }

        avl_update(node); // Maintain height and count

        uint32_t l = avl_height(node->left);
        uint32_t r = avl_height(node->right);

        if (l == r + 2) {
            *from = avl_fix_left(node); // Left-heavy
        } else if (l + 2 == r) {
            *from = avl_fix_right(node); // Right-heavy
        }

        if (!parent) return *from; // Stop if reached the root
        node = parent; // Move upward
    }
}

// Delete a node with at most one child and return new root
static AVLNode *avl_del_easy(AVLNode *node) {
    assert(!node->left || !node->right); // At most one child

    AVLNode *child = node->left ? node->left : node->right;
    AVLNode *parent = node->parent;

    if (child) child->parent = parent; // Update child's parent

    if (!parent) return child; // If deleting root, return new root

    AVLNode **from = parent->left == node ? &parent->left : &parent->right;
    *from = child;

    return avl_fix(parent); // Rebalance from parent
}

// Delete a node with two children and return new root
AVLNode *avl_del(AVLNode *node) {
    if (!node->left || !node->right) {
        return avl_del_easy(node); // Trivial case
    }

    // Find successor (smallest in right subtree)
    AVLNode *victim = node->right;
    while (victim->left) {
        victim = victim->left;
    }

    // Remove the successor
    AVLNode *root = avl_del_easy(victim);

    // Copy victim's contents into the node being deleted
    *victim = *node;

    // Fix parent pointers of victim's children
    if (victim->left) victim->left->parent = victim;
    if (victim->right) victim->right->parent = victim;

    AVLNode **from = &root;
    AVLNode *parent = node->parent;
    if (parent) {
        from = parent->left == node ? &parent->left : &parent->right;
    }
    *from = victim;

    return root;
}

// Traverse AVL tree from a node by a positional offset (in-order order)
AVLNode *avl_offset(AVLNode *node, int64_t offset) {
    int64_t pos = 0; // Position relative to starting node

    while (offset != pos) {
        if (pos < offset && pos + avl_cnt(node->right) >= offset) {
            // Target is in right subtree
            node = node->right;
            pos += avl_cnt(node->left) + 1;
        } else if (pos > offset && pos - avl_cnt(node->left) <= offset) {
            // Target is in left subtree
            node = node->left;
            pos -= avl_cnt(node->right) + 1;
        } else {
            // Move upward to parent
            AVLNode *parent = node->parent;
            if (!parent) return NULL;

            if (parent->right == node) {
                pos -= avl_cnt(node->left) + 1;
            } else {
                pos += avl_cnt(node->right) + 1;
            }
            node = parent;
        }
    }

    return node;
}
