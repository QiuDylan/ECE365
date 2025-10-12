#include "heap.h"
#include <cstddef>
#include <string>
#include <vector> 

void heap::percolate_down(std::size_t pos)
{
    nodes[0] = nodes[pos];
    auto prv = pos;

    while (pos << 1 <= use) {
        pos <<= 1;

        if ((pos < use) && nodes[pos + 1].key < nodes[pos].key) ++pos;

        if (nodes[0].key > nodes[pos].key) {
            nodes[prv] = nodes[pos];
            map->setPointer(nodes[prv].id, &nodes[prv]);
        } else break;

        prv = pos;
    }

    nodes[prv] = nodes[0];
    map->setPointer(nodes[prv].id, &nodes[prv]);
}

void heap::percolate_up(std::size_t pos)
{
    nodes[0] = nodes[pos];
    auto prv = pos;

    while (pos >> 1 > 0) {
        pos >>= 1;

        if (nodes[0].key < nodes[pos].key) {
            nodes[prv] = nodes[pos];
            map->setPointer(nodes[prv].id, &nodes[prv]);
        } else break;

        prv = pos;
    }

    nodes[prv] = nodes[0];
    map->setPointer(nodes[prv].id, &nodes[prv]);
}

heap::heap(std::size_t siz)
{
    map = new hashTable(siz * 2);
    nodes.resize(siz + 1);
    this->siz = siz;
    use       = 0;
}

heap::~heap(void)
{
	delete map;
}


int heap::insert(const std::string &id, int key, void *val)
{
    if (use + 1 > siz) return 1;
    if (map->contains(id)) return 2;

    std::size_t pos = ++use;
    nodes[pos].id  = id;
    nodes[pos].key = key;
    nodes[pos].val = val;
    map->insert(id, &nodes[pos]);

    if (use > 1) {
        percolate_up(pos);
    }
    return 0;
}

int heap::setKey(const std::string &id, int key)
{
    node_t *p = static_cast<node_t*>(map->getPointer(id));
    if (p == nullptr) return 1;

    size_t pos = p - &nodes[0];
    int prv_key    = nodes[pos].key;
    nodes[pos].key = key;

    if (key < prv_key)
        percolate_up(pos);
    else
        percolate_down(pos);
    return 0;
}

int heap::deleteMin(std::string *id, int *key, void **val)
{
    if (!use) return 1;

    if (id)  *id  = nodes[1].id;
    if (key) *key = nodes[1].key;
    if (val) *val = nodes[1].val;

    map->remove(nodes[1].id);
    
    if (use > 1) {
        nodes[1] = nodes[use--];
        // FIXED: Removed the setPointer call here.
        // The subsequent percolate_down call is now solely
        // responsible for updating the map for the moved node.
        percolate_down(1);
    } else {
        use--;
    }
    return 0;
}

int heap::remove(const std::string &id, int *key, void **val)
{
    node_t *p = static_cast<node_t*>(map->getPointer(id));
    if (p == nullptr) return 1;

    if (key) *key = p->key;
    if (val) *val = p->val;

    size_t pos = p - &nodes[0];
    map->remove(id);

    if (pos != use) {
        nodes[pos] = nodes[use--];
        // FIXED: Removed the setPointer call here.
        // The percolation functions will handle the pointer update.
        if ((pos > 1) && (nodes[pos].key < nodes[pos >> 1].key))
            percolate_up(pos);
        else
            percolate_down(pos);
    } else {
        use--;
    }
    return 0;
}