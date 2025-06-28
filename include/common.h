// common.h
#pragma once

#include <stdint.h>
#include <stddef.h>

/*
 * container_of Macro
 * ------------------
 * Retrieves a pointer to the parent structure from a pointer to one of its members.
 * Common in intrusive data structures (linked lists, AVL trees, etc).
 *
 * Parameters:
 *   ptr    - Pointer to the member
 *   type   - Type of the parent structure
 *   member - Name of the member inside the parent
 *
 * Example usage:
 *   struct MyStruct { int x; Node node; };
 *   Node* n = &obj.node;
 *   MyStruct* obj = container_of(n, MyStruct, node);
 *
 * NOTE: This macro uses GCC/Clang-specific 'typeof'.
 */
#define container_of(ptr, type, member) ({                      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);        \
    (type *)( (char *)__mptr - offsetof(type, member) );        \
})

/*
 * str_hash Function
 * -----------------
 * A lightweight 32-bit FNV-style hash function for byte arrays.
 *
 * Parameters:
 *   data - Pointer to the input byte array
 *   len  - Length of the array
 *
 * Returns:
 *   A 64-bit integer (though internally it uses a 32-bit accumulator).
 *   Useful for quick hash table indexing.
 */
inline uint64_t str_hash(const uint8_t *data, size_t len) {
    uint32_t h = 0x811C9DC5;
    for (size_t i = 0; i < len; i++) {
        h = (h + data[i]) * 0x01000193;
    }
    return h;
}
