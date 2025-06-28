// mock_client.cpp
/**
 * Mock Redis Client for Testing
 * =============================
 * A simple mock client that simulates Redis commands without needing the full server.
 * This demonstrates what the Python test script expects to see.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "zset.h"

// Global storage for demonstration
std::map<std::string, ZSet> zsets;

void print_nil() {
    std::cout << "(nil)" << std::endl;
}

void print_int(int value) {
    std::cout << "(int) " << value << std::endl;
}

void print_dbl(double value) {
    std::cout << "(dbl) " << value << std::endl;
}

void print_str(const std::string& value) {
    std::cout << "(str) " << value << std::endl;
}

void cmd_zscore(const std::string& key, const std::string& member) {
    auto it = zsets.find(key);
    if (it == zsets.end()) {
        print_nil();
        return;
    }
    
    ZNode* node = zset_lookup(&it->second, member.c_str(), member.length());
    if (node) {
        print_dbl(node->score);
    } else {
        print_nil();
    }
}

void cmd_zadd(const std::string& key, double score, const std::string& member) {
    ZSet& zset = zsets[key];  // Create if doesn't exist
    bool inserted = zset_insert(&zset, member.c_str(), member.length(), score);
    print_int(inserted ? 1 : 0);
}

void cmd_zquery(const std::string& key, const std::string& name, double score, int offset, int limit) {
    auto it = zsets.find(key);
    if (it == zsets.end()) {
        std::cout << "(arr) len=0" << std::endl;
        std::cout << "(arr) end" << std::endl;
        return;
    }
    
    ZNode* node = zset_seekge(&it->second, score, name.c_str(), name.length());
    std::vector<ZNode*> results;
    
    // Skip offset items
    for (int i = 0; i < offset && node; i++) {
        node = znode_offset(node, 1);
    }
    
    // Collect limit items
    for (int i = 0; i < limit && node; i++) {
        results.push_back(node);
        node = znode_offset(node, 1);
    }
    
    std::cout << "(arr) len=" << (results.size() * 2) << std::endl;
    for (ZNode* n : results) {
        std::string name_str(n->name, n->len);
        print_str(name_str);
        print_dbl(n->score);
    }
    std::cout << "(arr) end" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: client <command> [args...]" << std::endl;
        return 1;
    }
    
    std::string cmd = argv[1];
    
    if (cmd == "zscore" && argc == 4) {
        cmd_zscore(argv[2], argv[3]);
    }
    else if (cmd == "zadd" && argc == 5) {
        double score = std::stod(argv[3]);
        cmd_zadd(argv[2], score, argv[4]);
    }
    else if (cmd == "zquery" && argc == 7) {
        std::string name = argv[4];
        double score = std::stod(argv[5]);
        int offset = std::stoi(argv[6]);
        int limit = std::stoi(argv[7]);
        cmd_zquery(argv[2], name, score, offset, limit);
    }
    else {
        std::cerr << "Unknown command or wrong number of arguments" << std::endl;
        return 1;
    }
    
    return 0;
}
