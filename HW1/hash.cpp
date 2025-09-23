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
    
    if (contains(key)) {
        return 1;
    }
    
    // Check if rehash is needed
    if (filled >= capacity / 2) {
        if (!rehash()) {
            return 2; 
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
    
    return 0; 
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
        return 0; 
    }
    return 1; 
}

// Remove function
bool hashTable::remove(const std::string &key) {
    int pos = findPos(key);
    if (pos != -1) {
        data[pos].isDeleted = true;
        return true;
    }
    return false; 
}

// Hash function
int hashTable::hash(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key) % capacity;
}

// Find position function
int hashTable::findPos(const std::string &key) {
    int pos = hash(key);
    
    while (data[pos].isOccupied) {
        if (!data[pos].isDeleted && data[pos].key == key) {
            return pos;
        }
        pos = (pos + 1) % capacity;
        
        if (pos == hash(key)) {
            break;
        }
    }
    
    return -1; 
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
                data = oldData;
                capacity = oldCapacity;
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

// Get prime function
unsigned int hashTable::getPrime(int size) {
    // Prime numbers
    static const unsigned int primes[] = {
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
        6291469, 12582917, 25165843, 50331653, 100663319,
        201326611, 402653189, 805306457, 1610612741,3221225533
    };
    
    const int numPrimes = sizeof(primes) / sizeof(primes[0]);
    
    for (int i = 0; i < numPrimes; i++) {
        if (primes[i] >= size) {
            return primes[i];
        }
    }
    return primes[numPrimes - 1];
}