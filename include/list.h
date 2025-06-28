// list.h
#pragma once

#include <stddef.h>

/*
 * DList - Doubly Linked List Node
 * --------------------------------
 * This is an intrusive doubly linked list node.
 * It is meant to be embedded inside user-defined structures.
 *
 * Example usage:
 *   struct MyObject {
 *       DList link;
 *       int data;
 *   };
 *
 *   You can insert/remove/link `MyObject` instances via their `link` field.
 */
struct DList {
    DList *prev = nullptr;
    DList *next = nullptr;
};

/*
 * dlist_init - Initialize a DList node to point to itself
 * -------------------------------------------------------
 * This is used to mark an empty list or set a node to be ready for linking.
 * It creates a circular reference (node.prev = node.next = node).
 */
inline void dlist_init(DList *node) {
    node->prev = node->next = node;
}

/*
 * dlist_empty - Check if the list is empty
 * ----------------------------------------
 * Returns true if `node` is the only node in the list (i.e., list is empty).
 */
inline bool dlist_empty(DList *node) {
    return node->next == node;
}

/*
 * dlist_detach - Remove a node from the list
 * ------------------------------------------
 * Unlinks `node` from its current list.
 * After this, the node is no longer part of the list.
 */
inline void dlist_detach(DList *node) {
    DList *prev = node->prev;
    DList *next = node->next;
    prev->next = next;
    next->prev = prev;
}

/*
 * dlist_insert_before - Insert a node before a target node
 * --------------------------------------------------------
 * Inserts `rookie` node before the `target` node in the list.
 * The list must already be initialized.
 */
inline void dlist_insert_before(DList *target, DList *rookie) {
    DList *prev = target->prev;
    prev->next = rookie;
    rookie->prev = prev;
    rookie->next = target;
    target->prev = rookie;
}
