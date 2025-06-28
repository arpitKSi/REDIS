# Custom Redis Implementation

A lightweight, high-performance Redis-like key-value store implementation in C++ featuring advanced data structures and Redis-compatible operations.

## 🚀 Features

- **AVL Tree**: Self-balancing binary search tree with O(log n) operations
- **Hash Table**: Progressive rehashing with O(1) average lookup time  
- **Binary Heap**: Min-heap implementation for priority queue operations
- **Sorted Sets (ZSet)**: Dual structure combining AVL tree + hash table for fast lookups and range queries
- **Thread Pool**: POSIX threads-based worker pool for concurrent operations
- **Redis Commands**: `ZADD`, `ZSCORE`, `ZQUERY`, `ZRANK` with range queries and rankings

## 📁 Project Structure

```
REDIS/
├── include/           # Header files
│   ├── avl.h         # AVL tree implementation
│   ├── common.h      # Common utilities and macros
│   ├── hashtable.h   # Hash table with progressive rehashing
│   ├── heap.h        # Binary heap implementation
│   ├── list.h        # Doubly linked list utilities
│   ├── thread_pool.h # Thread pool for concurrency
│   └── zset.h        # Sorted set (ZSet) implementation
├── src/              # Source files
│   ├── 14_server.cpp # Main Redis server (requires Unix/Linux)
│   ├── avl.cpp       # AVL tree operations
│   ├── hashtable.cpp # Hash table implementation
│   ├── heap.cpp      # Heap operations
│   ├── thread_pool.cpp # Thread pool implementation
│   └── zset.cpp      # Sorted set operations
├── tests/            # Test suite
│   ├── test_avl.cpp  # AVL tree comprehensive tests
│   ├── test_heap.cpp # Heap functionality tests
│   ├── test_offset.cpp # AVL offset/ranking tests
│   └── test_cmds.py  # Redis command integration tests
├── demo.cpp          # Demonstration program
├── Makefile          # Build configuration
└── README.md         # This file
```

## 🚀 Quick Start

```bash
# Clone and build
git clone https://github.com/arpitKSi/REDIS.git
cd REDIS
make

# Run the demo
./demo.exe
```

## 📺 Demo Output

```
=== Custom Redis Demo ===

1. Testing Sorted Set (ZSet) functionality...
   Adding players with scores...
   Looking up scores...
   Alice's score: 100.5
   Bob's score: 85
   Updating Alice's score to 95.0...
   Alice's new score: 95
   Finding players with score >= 90.0...
   Player: charlie (score: 92.3)
   Player: alice (score: 95)
   Player: diana (score: 110.2)

2. Testing Hash Table functionality...
   Hash table is working internally in ZSet!
   ZSet size: 4 players

3. Testing AVL Tree functionality...
   AVL tree is maintaining sorted order in ZSet!
   Tree height: 3
   Total nodes: 4

=== Demo completed successfully! ===
```

## 🎮 Usage Examples

### Redis-Like Commands

```bash
# Build mock client
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude mock_client.cpp src/avl.o src/hashtable.o src/zset.o -o client.exe

# Add members to sorted set
./client.exe zadd leaderboard 100 alice    # Returns: (int) 1
./client.exe zadd leaderboard 85 bob       # Returns: (int) 1

# Get scores
./client.exe zscore leaderboard alice      # Returns: (dbl) 100
./client.exe zscore leaderboard bob        # Returns: (dbl) 85
```

### Programming API

```cpp
#include "zset.h"

int main() {
    ZSet leaderboard = {};
    
    // Add players with scores
    zset_insert(&leaderboard, "alice", 5, 100.0);
    zset_insert(&leaderboard, "bob", 3, 85.0);
    
    // Look up player's score
    ZNode* player = zset_lookup(&leaderboard, "alice", 5);
    if (player) {
        printf("Alice's score: %.1f\n", player->score);
    }
    
    // Find players with score >= 90
    ZNode* node = zset_seekge(&leaderboard, 90.0, "", 0);
    while (node) {
        printf("High scorer: %.*s (%.1f)\n", 
               (int)node->len, node->name, node->score);
        node = znode_offset(node, 1);
    }
    
    zset_clear(&leaderboard);
    return 0;
}
```

## 🛠️ Building

### Prerequisites
- **C++17 Compiler**: GCC 7+ or Clang 6+ with GNU extensions
- **POSIX Threads**: For thread pool functionality

### Build Commands

```bash
# Using Makefile
make                 # Build all components
make clean          # Clean build artifacts

# Manual compilation
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/avl.cpp -o src/avl.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/hashtable.cpp -o src/hashtable.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/zset.cpp -o src/zset.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude demo.cpp src/avl.o src/hashtable.o src/zset.o -o demo.exe
```

## 🧪 Testing

```bash
# Run all tests
./test_avl.exe       # AVL tree functionality (500+ test cases)
./test_offset.exe    # AVL offset/ranking operations  
./test_heap.exe      # Binary heap operations
python test_redis_commands.py  # Redis command compatibility
```

**Sample Test Output:**
```bash
$ python test_redis_commands.py
=== Mock Redis Command Test ===
Test 1 - zscore asdf n1: PASS (nil)
Test 2 - zadd zset 1 n1: PASS (int) 1
Test 3 - zadd zset 2 n2: PASS (int) 1
Test 4 - zscore zset n1: PASS (dbl) 1
=== All tests completed ===
```

## 🔧 Architecture

- **Intrusive Design**: Nodes embedded in user structures for memory efficiency
- **Dual Indexing**: ZSet uses both AVL tree (ordering) and hash table (fast lookups)
- **Progressive Rehashing**: Hash table grows incrementally to avoid blocking
- **Self-Balancing**: AVL tree maintains O(log n) height automatically

## 🏗️ Platform Support

- **Linux/macOS**: Complete functionality including networking server
- **Windows**: All data structures work; networking server requires Unix headers

## 🔧 Troubleshooting

**Common Issues:**
```bash
# Issue: cannot find 'typeof'  
# Solution: Use GNU extensions
g++ -std=gnu++17 -Wall -Wno-unused-variable ...

# Issue: undefined reference to pthread_create
# Solution: Link pthread library (Linux/macOS)
g++ ... -lpthread
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is open source. See individual file headers for licensing information.

---

**Built with ❤️ for learning advanced data structures and Redis internals**
