#include <cstddef>
#include <string>
#include <vector>

#include "hash.h"
class heap {
      
    typedef struct node_s {
        std::string id;
        int key;
        void *val;
    } node_t;

    static inline const void  *stringkey(const std::string &key);
    static inline std::size_t  stringsiz(const std::string &key);

    void percolate_down(std::size_t pos);
    void percolate_up(std::size_t pos);

    std::size_t siz; //capacity
    std::size_t use; //number of slots currently used
    hashTable *map; //hashtable
    std::vector<node_t> nodes; //actual binary heap

    public:
        heap(std::size_t siz = 1);
        ~heap(void);

        int deleteMin(
            std::string  *id  = nullptr,
            int *key = nullptr,
            void **val = nullptr
        );
        int insert(const std::string &id, int key, void *val = nullptr);
        int setKey(const std::string &id, int key);
        int remove(
            const std::string  &id,
            int *key = nullptr,
            void **val = nullptr
        );
};

