#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> 
#include <limits>
#include "hash.h"
using namespace std;

// Constructor
hashTable::hashTable(int size) {
    capacity = getPrime(size);
    filled = 0;
    data.resize(capacity);
}

// Insert function
int hashTable::insert(const std::string &key, void *pv) {
    // Check if key already exists
    if (contains(key)) {
        return 1;
    }
    
    // Check if rehash is needed (load factor > 0.5)
    if (filled >= capacity / 2) {
        if (!rehash()) {
            return 2; // Rehash failed
        }
    }
    
    // Find position using linear probing
    int pos = hash(key);
    while (data[pos].isOccupied && !data[pos].isDeleted) {
        pos = (pos + 1) % capacity;
    }
    
    // Insert the item
    data[pos].key = key;
    data[pos].isOccupied = true;
    data[pos].isDeleted = false;
    data[pos].pv = pv;
    filled++;
    
    return 0; // Success
}

// Contains function
bool hashTable::contains(const std::string &key) {
    return findPos(key) != -1;
}

// Get pointer function
void *hashTable::getPointer(const std::string &key, bool *b) {
    int pos = findPos(key);
    if (pos != -1) {
        if (b != nullptr) {
            *b = true;
        }
        return data[pos].pv;
    } else {
        if (b != nullptr) {
            *b = false;
        }
        return nullptr;
    }
}

// Set pointer function
int hashTable::setPointer(const std::string &key, void *pv) {
    int pos = findPos(key);
    if (pos != -1) {
        data[pos].pv = pv;
        return 0; // Success
    }
    return 1; // Key not found
}

// Remove function
bool hashTable::remove(const std::string &key) {
    int pos = findPos(key);
    if (pos != -1) {
        data[pos].isDeleted = true;
        return true;
    }
    return false; // Key not found
}

// Hash function - uses std::hash for strings
int hashTable::hash(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key) % capacity;
}

// Find position function
int hashTable::findPos(const std::string &key) {
    int pos = hash(key);
    
    while (data[pos].isOccupied) {
        if (!data[pos].isDeleted && data[pos].key == key) {
            return pos; // Found the key
        }
        pos = (pos + 1) % capacity;
        
        // If we've gone full circle, the key is not in the table
        if (pos == hash(key)) {
            break;
        }
    }
    
    return -1; // Key not found
}

// Rehash function
bool hashTable::rehash() {
    // Save old data
    std::vector<hashItem> oldData = data;
    int oldCapacity = capacity;
    
    // Create new, larger table
    capacity = getPrime(capacity * 2);
    filled = 0;
    data.clear();
    data.resize(capacity);
    
    // Reinsert all non-deleted items
    for (int i = 0; i < oldCapacity; i++) {
        if (oldData[i].isOccupied && !oldData[i].isDeleted) {
            if (insert(oldData[i].key, oldData[i].pv) != 0) {
                // If insertion fails, restore old table
                data = oldData;
                capacity = oldCapacity;
                // Recalculate filled count
                filled = 0;
                for (int j = 0; j < capacity; j++) {
                    if (data[j].isOccupied && !data[j].isDeleted) {
                        filled++;
                    }
                }
                return false;
            }
        }
    }
    
    return true;
}

// Get prime function - returns a prime number at least as large as size
unsigned int hashTable::getPrime(int size) {
    // Precomputed prime numbers
    static const unsigned int primes[] = {
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
        6291469, 12582917, 25165843, 50331653, 100663319,
        201326611, 402653189, 805306457, 1610612741
    };
    
    const int numPrimes = sizeof(primes) / sizeof(primes[0]);
    
    for (int i = 0; i < numPrimes; i++) {
        if (primes[i] >= size) {
            return primes[i];
        }
    }
    
    // If size is larger than our largest prime, return the largest prime
    return primes[numPrimes - 1];
}