#!/usr/bin/env python3
"""
Mock Redis Command Test
=======================
Tests Redis commands using our mock client in a single session.
"""

import subprocess
import tempfile
import os

def run_redis_test():
    print("=== Mock Redis Command Test ===")
    
    # Create a simple test script that tests our Redis functionality
    test_script = '''
#include <iostream>
#include <cassert>
#include "zset.h"

int main() {
    ZSet zset = {};
    
    // Test 1: zscore on non-existent key should return nil
    ZNode* node = zset_lookup(&zset, "n1", 2);
    std::cout << "Test 1 - zscore asdf n1: ";
    std::cout << (node ? "FAIL" : "PASS (nil)") << std::endl;
    
    // Test 2: zadd should return 1 for new member
    bool added = zset_insert(&zset, "n1", 2, 1.0);
    std::cout << "Test 2 - zadd zset 1 n1: ";
    std::cout << (added ? "PASS (int) 1" : "FAIL") << std::endl;
    
    // Test 3: zadd should return 1 for another new member  
    added = zset_insert(&zset, "n2", 2, 2.0);
    std::cout << "Test 3 - zadd zset 2 n2: ";
    std::cout << (added ? "PASS (int) 1" : "FAIL") << std::endl;
    
    // Test 4: zadd should return 0 for updating existing member
    added = zset_insert(&zset, "n1", 2, 1.1);
    std::cout << "Test 4 - zadd zset 1.1 n1 (update): ";
    std::cout << (!added ? "PASS (int) 0" : "FAIL") << std::endl;
    
    // Test 5: zscore should return the updated score
    node = zset_lookup(&zset, "n1", 2);
    std::cout << "Test 5 - zscore zset n1: ";
    if (node && node->score == 1.1) {
        std::cout << "PASS (dbl) 1.1" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
    
    // Test 6: Check total count
    std::cout << "Test 6 - Total members: ";
    std::cout << hm_size(&zset.hmap) << " (expected: 2)" << std::endl;
    
    // Test 7: Range query test
    std::cout << "Test 7 - Range query (score >= 1.0):" << std::endl;
    node = zset_seekge(&zset, 1.0, "", 0);
    int count = 0;
    while (node && count < 5) {
        std::cout << "  Member: ";
        for (size_t i = 0; i < node->len; i++) {
            std::cout << node->name[i];
        }
        std::cout << " (score: " << node->score << ")" << std::endl;
        node = znode_offset(node, 1);
        count++;
    }
    
    zset_clear(&zset);
    std::cout << "\\n=== All tests completed ===" << std::endl;
    return 0;
}
'''
    
    # Write test to temporary file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.cpp', delete=False) as f:
        f.write(test_script)
        test_file = f.name
    
    try:
        # Compile the test
        compile_cmd = [
            'g++', '-std=gnu++17', '-Wall', '-Wno-unused-variable', 
            '-Iinclude', test_file, 'src/avl.o', 'src/hashtable.o', 'src/zset.o',
            '-o', 'redis_test.exe'
        ]
        
        print("Compiling Redis test...")
        result = subprocess.run(compile_cmd, capture_output=True, text=True, cwd='.')
        if result.returncode != 0:
            print(f"Compilation failed: {result.stderr}")
            return
        
        # Run the test
        print("Running Redis command tests...")
        result = subprocess.run(['./redis_test.exe'], capture_output=True, text=True, cwd='.')
        print(result.stdout)
        
        if result.returncode != 0:
            print(f"Test execution failed: {result.stderr}")
        
    finally:
        # Clean up
        try:
            os.unlink(test_file)
            os.unlink('redis_test.exe')
        except:
            pass

if __name__ == '__main__':
    run_redis_test()
