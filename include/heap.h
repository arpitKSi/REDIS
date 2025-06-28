// heap.h
#pragma once

#include <stddef.h>
#include <stdint.h>

/*
 * HeapItem - Represents an element in a binary heap.
 * ---------------------------------------------------
 * This structure is used for implementing a priority queue or heap.
 * Each item has:
 *   - val : The key value used for ordering in the heap.
 *   - ref : Optional pointer to an external variable (typically used to store
 *           the current index of this item in the heap for fast access).
 */
struct HeapItem {
    uint64_t val = 0;     // Priority value
    size_t *ref = nullptr; // Pointer to external index tracking
};

/*
 * heap_update - Heapify the element at position `pos` in array `a` of length `len`
 * -------------------------------------------------------------------------------
 * This function fixes the heap order by performing a sift-up or sift-down operation
 * depending on where the item at position `pos` needs to go.
 *
 * Parameters:
 *   a   : The array of HeapItem elements (heap)
 *   pos : The index of the element to adjust
 *   len : The number of elements in the heap
 *
 * Notes:
 * - This likely implements an in-place binary heap (min or max) stored as an array.
 * - The implementation will probably maintain the heap invariant:
 *     a[parent(i)] <= a[i]   or   a[parent(i)] >= a[i]
 */
void heap_update(HeapItem *a, size_t pos, size_t len);
