#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> 
#include <limits>
#include "hash.h"
using namespace std;


static const unsigned int PRIME_LIST[] = {
    53u,         97u,         193u,        389u,        769u,
    1543u,       3079u,       6151u,       12289u,      24593u,
    49157u,      98317u,      196613u,     393241u,     786433u,
    1572869u,    3145739u,    6291469u,    12582917u,   25165843u,
    50331653u,   100663319u,  201326611u,  402653189u,  805306457u,
    1610612741u
};
static const size_t PRIME_COUNT =
    sizeof(PRIME_LIST)/sizeof(PRIME_LIST[0]);

// -----------------------------
// Constructor
// -----------------------------
hashTable::hashTable(int size)
{
  if (size < 0) size = 0;
  capacity = static_cast<int>(getPrime(size));
  data.resize(capacity);
  filled = 0;
}

// -----------------------------
// hash function (returns int in [0,capacity-1])
// polynomial rolling hash
// -----------------------------
int hashTable::hash(const std::string &key)
{
  unsigned long long h = 0;
  for (char ch : key) {
    // key expected to be lowercase already by caller, but hash doesn't require that
    h = h * 37ull + static_cast<unsigned char>(ch);
  }
  // avoid overflow problems (but unsigned will wrap naturally)
  return static_cast<int>(h % static_cast<unsigned long long>(capacity));
}

// -----------------------------
// findPos: return index if key found, -1 otherwise
// (used for search/getPointer/setPointer/remove)
// -----------------------------
int hashTable::findPos(const std::string &key)
{
  if (capacity == 0) return -1;
  int idx = hash(key);
  int start = idx;
  while (true) {
    const hashItem &it = data[idx];
    if (!it.isOccupied) {
      // empty slot — key cannot be present
      return -1;
    }
    if (!it.isDeleted && it.key == key) {
      return idx;
    }
    idx++;
    if (idx >= capacity) idx = 0;
    if (idx == start) break; // full circle
  }
  return -1;
}

// -----------------------------
// insert
// Returns:
// 0 on success
// 1 if key already exists
// 2 if rehash fails
// -----------------------------
int hashTable::insert(const std::string &key, void *pv)
{
  if (capacity == 0) {
    // initialize to a small prime if table was created with size 0
    capacity = static_cast<int>(getPrime(1));
    data.clear();
    data.resize(capacity);
    filled = 0;
  }

  // If load factor would exceed 0.5, rehash first
  if ( (filled + 1) * 2 > capacity ) {
    if (!rehash()) {
      return 2; // rehash failed
    }
  }

  // Check if key already present
  if (contains(key)) return 1;

  // linear probe to find insert position; prefer first deleted slot
  int idx = hash(key);
  int firstDeleted = -1;
  while (true) {
    hashItem &it = data[idx];
    if (!it.isOccupied) {
      // empty slot, use it (or earlier deleted)
      int useIdx = (firstDeleted != -1) ? firstDeleted : idx;
      hashItem &dest = data[useIdx];
      dest.key = key;
      dest.isOccupied = true;
      dest.isDeleted = false;
      dest.pv = pv;
      filled++;
      return 0;
    } else if (it.isDeleted) {
      if (firstDeleted == -1) firstDeleted = idx;
    } else {
      if (it.key == key) {
        // should not happen because we checked contains above, but keep safe
        return 1;
      }
    }
    idx++;
    if (idx >= capacity) idx = 0;
  }

  // unreachable
  return 2;
}

// -----------------------------
// contains
// -----------------------------
bool hashTable::contains(const std::string &key)
{
  return (findPos(key) != -1);
}

// -----------------------------
// getPointer
// If key not found, returns nullptr.
// If bool* b provided, sets to true if found, false otherwise.
// -----------------------------
void *hashTable::getPointer(const std::string &key, bool *b)
{
  int pos = findPos(key);
  if (b) {
    *b = (pos != -1);
  }
  if (pos == -1) return nullptr;
  return data[pos].pv;
}

// -----------------------------
// setPointer
// Returns 0 on success, 1 if key doesn't exist
// -----------------------------
int hashTable::setPointer(const std::string &key, void *pv)
{
  int pos = findPos(key);
  if (pos == -1) return 1;
  data[pos].pv = pv;
  return 0;
}

// -----------------------------
// remove
// Returns true on success, false if key not found
// -----------------------------
bool hashTable::remove(const std::string &key)
{
  int pos = findPos(key);
  if (pos == -1) return false;
  // lazy deletion
  if (!data[pos].isDeleted) {
    data[pos].isDeleted = true;
    // keep isOccupied = true (slot was occupied) but decrement filled count
    if (filled > 0) filled--;
  }
  return true;
}

// -----------------------------
// rehash: make table bigger (next prime at least double current capacity)
// Returns true on success, false on bad_alloc
// -----------------------------
bool hashTable::rehash()
{
  // find new capacity: next prime >= 2 * capacity
  unsigned int newCap = getPrime(capacity * 2);
  if (newCap == 0) return false;

  std::vector<hashItem> oldData;
  oldData.swap(data); // move out
  int oldCap = capacity;

  try {
    data.clear();
    data.resize(static_cast<size_t>(newCap));
  } catch (const std::bad_alloc &) {
    // restore old data
    data.swap(oldData);
    capacity = oldCap;
    return false;
  }

  capacity = static_cast<int>(newCap);
  filled = 0;

  // reinsert old entries that are occupied and not deleted
  for (int i = 0; i < oldCap; ++i) {
    const hashItem &it = oldData[i];
    if (it.isOccupied && !it.isDeleted) {
      // Insert into new table (we can bypass rehash check inside insert
      // by performing a direct linear probe here)
      int idx = hash(it.key);
      while (true) {
        hashItem &dest = data[idx];
        if (!dest.isOccupied) {
          dest.key = it.key;
          dest.isOccupied = true;
          dest.isDeleted = false;
          dest.pv = it.pv;
          filled++;
          break;
        }
        idx++;
        if (idx >= capacity) idx = 0;
      }
    }
  }

  return true;
}

// -----------------------------
// getPrime: return prime >= size using list above
// If size <= 0, return first prime
// -----------------------------
unsigned int hashTable::getPrime(int size)
{
  if (size <= 0) return PRIME_LIST[0];
  for (size_t i = 0; i < PRIME_COUNT; ++i) {
    if (PRIME_LIST[i] >= static_cast<unsigned int>(size)) {
      return PRIME_LIST[i];
    }
  }
  // if requested size is bigger than our largest precomputed prime,
  // just return the largest precomputed prime
  return PRIME_LIST[PRIME_COUNT - 1];
}
