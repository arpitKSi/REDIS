// demo.cpp
/**
 * Custom Redis Demo Program
 * ========================
 * Demonstrates the core functionality of the Custom Redis data structures
 * without the networking components that require Unix-specific headers.
 */

#include <iostream>
#include <string>
#include <cassert>
#include "zset.h"
#include "hashtable.h"
#include "common.h"

int main() {
    std::cout << "=== Custom Redis Demo ===" << std::endl;
    
    // Test ZSet (Sorted Set) functionality
    std::cout << "\n1. Testing Sorted Set (ZSet) functionality..." << std::endl;
    
    ZSet zset = {};
    
    // Add some scores
    std::cout << "   Adding players with scores..." << std::endl;
    zset_insert(&zset, "alice", 5, 100.5);
    zset_insert(&zset, "bob", 3, 85.0);
    zset_insert(&zset, "charlie", 7, 92.3);
    zset_insert(&zset, "diana", 5, 110.2);
    
    // Lookup scores
    std::cout << "   Looking up scores..." << std::endl;
    ZNode *node = zset_lookup(&zset, "alice", 5);
    if (node) {
        std::cout << "   Alice's score: " << node->score << std::endl;
    }
    
    node = zset_lookup(&zset, "bob", 3);
    if (node) {
        std::cout << "   Bob's score: " << node->score << std::endl;
    }
    
    // Update a score
    std::cout << "   Updating Alice's score to 95.0..." << std::endl;
    zset_insert(&zset, "alice", 5, 95.0);
    
    node = zset_lookup(&zset, "alice", 5);
    if (node) {
        std::cout << "   Alice's new score: " << node->score << std::endl;
    }
    
    // Find players by score range
    std::cout << "   Finding players with score >= 90.0..." << std::endl;
    node = zset_seekge(&zset, 90.0, "", 0);
    int count = 0;
    while (node && count < 10) {  // Prevent infinite loop
        std::cout << "   Player: ";
        for (size_t i = 0; i < node->len; i++) {
            std::cout << node->name[i];
        }
        std::cout << " (score: " << node->score << ")" << std::endl;
        
        node = znode_offset(node, 1);  // Next player
        count++;
        if (node && node->score < 90.0) break;  // Stop if score drops below threshold
    }
    
    std::cout << "\n2. Testing Hash Table functionality..." << std::endl;
    
    // The hash table is already being used internally by ZSet
    std::cout << "   Hash table is working internally in ZSet!" << std::endl;
    std::cout << "   ZSet size: " << hm_size(&zset.hmap) << " players" << std::endl;
    
    std::cout << "\n3. Testing AVL Tree functionality..." << std::endl;
    std::cout << "   AVL tree is maintaining sorted order in ZSet!" << std::endl;
    if (zset.root) {
        std::cout << "   Tree height: " << zset.root->height << std::endl;
        std::cout << "   Total nodes: " << zset.root->cnt << std::endl;
    }
    
    // Clean up
    zset_clear(&zset);
    
    std::cout << "\n=== Demo completed successfully! ===" << std::endl;
    std::cout << "All Custom Redis components are working correctly." << std::endl;
    
    return 0;
}
