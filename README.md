# Custom Redis Implementation

A lightweight, high-performance Redis-like key-value store implementation in C++ featuring advanced data structures and Redis-compatible operations.

## 🚀 Features

### Core Data Structures
- **AVL Tree**: Self-balancing binary search tree with O(log n) operations
- **Hash Table**: Progressive rehashing with O(1) average lookup time
- **Binary Heap**: Min-heap implementation for priority queue operations
- **Sorted Sets (ZSet)**: Dual structure combining AVL tree + hash table for fast lookups and range queries
- **Doubly Linked Lists**: Intrusive design for memory efficiency
- **Thread Pool**: POSIX threads-based worker pool for concurrent operations

### Redis-Compatible Operations
- `ZADD` - Add members to sorted sets with scores
- `ZSCORE` - Get score of a member in a sorted set
- `ZQUERY` - Range queries on sorted sets
- `ZRANK` - Get rank/position of members
- Hash table operations with progressive rehashing
- TTL (Time To Live) support for automatic key expiration

## 📁 Project Structure

```
Custom_Redis/
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

## 🛠️ Building the Project

### Prerequisites
- **C++ Compiler**: GCC 7+ or Clang 6+ with C++17 support
- **GNU Extensions**: Required for `typeof` and other extensions
- **POSIX Threads**: For thread pool functionality

### Build Commands

```bash
# Compile individual components
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/avl.cpp -o src/avl.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/hashtable.cpp -o src/hashtable.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/heap.cpp -o src/heap.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/zset.cpp -o src/zset.o
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -c src/thread_pool.cpp -o src/thread_pool.o

# Build demo program
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude demo.cpp src/avl.o src/hashtable.o src/zset.o -o demo.exe

# Build tests
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude tests/test_avl.cpp src/avl.o -o test_avl.exe
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude tests/test_offset.cpp src/avl.o -o test_offset.exe
g++ -std=gnu++17 -Wall -Wno-unused-variable -Iinclude -Isrc tests/test_heap.cpp -o test_heap.exe
```

### Using Makefile

```bash
make                 # Build all components
make clean          # Clean build artifacts
```

## 🧪 Testing

### Run All Tests

```bash
# Run core data structure tests
./test_avl.exe       # AVL tree functionality
./test_offset.exe    # AVL offset/ranking operations  
./test_heap.exe      # Binary heap operations

# Run demonstration
./demo.exe           # Interactive demo of Redis functionality
```

### Test Coverage
- **AVL Tree**: 500+ test cases covering insertion, deletion, balancing, and offset queries
- **Hash Table**: Progressive rehashing, collision handling, and performance tests
- **Heap**: Min-heap property maintenance and external reference tracking
- **ZSet**: Combined AVL + hash table operations with Redis command compatibility

## 🎯 Demo Output

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

## 🏗️ Architecture

### Data Structure Design
- **Intrusive Design**: Nodes are embedded in user structures for memory efficiency
- **Dual Indexing**: ZSet uses both AVL tree (for ordering) and hash table (for fast lookups)
- **Progressive Rehashing**: Hash table grows incrementally to avoid blocking operations
- **Self-Balancing**: AVL tree maintains O(log n) height automatically

### Performance Characteristics
- **Hash Table**: O(1) average lookup, O(n) worst case
- **AVL Tree**: O(log n) insertion, deletion, and search
- **ZSet Operations**: O(log n) for scoring operations, O(1) for member lookups
- **Memory**: Intrusive design minimizes memory overhead

## 🔧 Platform Support

### Fully Supported (All Features)
- **Linux**: Complete functionality including networking server
- **macOS**: Complete functionality including networking server
- **Unix-like systems**: Complete functionality

### Partially Supported (Core Features Only)
- **Windows**: All data structures and algorithms work perfectly
- **Limitation**: Networking server requires Unix-specific headers (`poll.h`, `sys/socket.h`)
- **Workaround**: Use demo program and tests to verify functionality

## 📚 Code Documentation

All code follows industry-standard documentation practices:
- **Function Documentation**: Parameters, return values, and behavior
- **Algorithm Explanations**: Time/space complexity and implementation notes
- **Usage Examples**: Demonstration of proper API usage
- **Thread Safety**: Synchronization patterns documented where applicable

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is open source. See individual file headers for specific licensing information.

## 🙏 Acknowledgments

- Inspired by Redis architecture and data structure design
- AVL tree implementation based on academic algorithms
- Hash table design follows industry best practices for progressive rehashing

---

**Built with ❤️ for learning advanced data structures and Redis internals**
