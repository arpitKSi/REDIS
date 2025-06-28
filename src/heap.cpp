// heap.cpp
#include "heap.h"

/**
 * heap_parent - Return the index of the parent of node at index i
 * @i: Index of the child node
 * 
 * In a binary heap stored as an array, the parent of element at index i
 * is located at index (i-1)/2. This function uses the equivalent formula
 * (i+1)/2 - 1 to avoid potential issues with integer division.
 * 
 * Returns: Index of the parent node
 */
static size_t heap_parent(size_t i) {
    return (i + 1) / 2 - 1;
}

/**
 * heap_left - Return the index of the left child of node at index i
 * @i: Index of the parent node
 * 
 * In a binary heap, the left child of element at index i is at index 2*i+1.
 * 
 * Returns: Index of the left child
 */
static size_t heap_left(size_t i) {
    return i * 2 + 1;
}

/**
 * heap_right - Return the index of the right child of node at index i
 * @i: Index of the parent node
 * 
 * In a binary heap, the right child of element at index i is at index 2*i+2.
 * 
 * Returns: Index of the right child
 */
static size_t heap_right(size_t i) {
    return i * 2 + 2;
}

/**
 * heap_up - Move an item up the heap to restore min-heap property
 * @a: Array representing the heap
 * @pos: Starting position of the item to move up
 * 
 * This function performs a "sift-up" or "bubble-up" operation. It moves
 * the item at position `pos` upward in the heap until the min-heap property
 * is satisfied (parent <= child).
 * 
 * This is typically used after inserting a new item at the end of the heap
 * or after decreasing the value of an existing item.
 * 
 * The function also updates the external reference pointers (ref field)
 * to track the new positions of moved items.
 */
static void heap_up(HeapItem *a, size_t pos) {
    HeapItem t = a[pos]; // Store the item to be moved up
    
    // Move up while parent exists and has a larger value
    while (pos > 0 && a[heap_parent(pos)].val > t.val) {
        // Move parent down to current position
        a[pos] = a[heap_parent(pos)];
        *a[pos].ref = pos;   // Update external pointer to this item's new position
        pos = heap_parent(pos);
    }
    
    // Place the item at its final position
    a[pos] = t;
    *a[pos].ref = pos;
}

// Move the item at `pos` down to restore heap property (min-heap)
// `a[pos]` is the item being updated or moved after deletion
static void heap_down(HeapItem *a, size_t pos, size_t len) {
    HeapItem t = a[pos]; // item to place
    while (true) {
        size_t l = heap_left(pos);
        size_t r = heap_right(pos);
        size_t min_pos = pos;
        uint64_t min_val = t.val;

        // Find smallest among parent and children
        if (l < len && a[l].val < min_val) {
            min_pos = l;
            min_val = a[l].val;
        }
        if (r < len && a[r].val < min_val) {
            min_pos = r;
        }

        if (min_pos == pos) break; // heap property restored

        // Swap with smallest child
        a[pos] = a[min_pos];
        *a[pos].ref = pos;
        pos = min_pos;
    }
    a[pos] = t;
    *a[pos].ref = pos;
}

// Update heap when a single element is changed at `pos`
// If the element decreased, bubble it up; else, bubble it down
void heap_update(HeapItem *a, size_t pos, size_t len) {
    if (pos > 0 && a[heap_parent(pos)].val > a[pos].val) {
        heap_up(a, pos);
    } else {
        heap_down(a, pos, len);
    }
}
