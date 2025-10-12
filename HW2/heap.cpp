#include "heap.h"
#include <cstddef>
#include <string>
#include <climits> 

inline const void *heap::stringid(const std::string &key)
{
	return key.data();
}

inline std::size_t heap::stringsiz(const std::string &key)
{
	return key.size();
}

void heap::percolate_down(std::size_t pos)
{
	nodes[0] = nodes[pos];
	auto prv = pos;

	while (pos << 1 <= use) {
		pos <<= 1;

		if ((pos < use) && nodes[pos + 1].key < nodes[pos].key) ++pos;

		if (nodes[0].key > nodes[pos].key) {
			nodes[prv] = nodes[pos];
			idmap->set(nodes[prv].id, &nodes[prv]);
		} else break;

		prv = pos;
	}

	nodes[prv] = nodes[0];
	idmap->set(nodes[prv].id, &nodes[prv]);
}

void heap::percolate_up(std::size_t pos)
{
	nodes[0] = nodes[pos];
	auto prv = pos;

	while (pos >> 1 > 0) {
		pos >>= 1;

		if (nodes[0].key < nodes[pos].key) {
			nodes[prv] = nodes[pos];
			idmap->set(nodes[prv].id, &nodes[prv]);
		} else break;

		prv = pos;
	}

	nodes[prv] = nodes[0];
	idmap->set(nodes[prv].id, &nodes[prv]);
}


heap::heap(std::size_t siz)
{
	idmap = new htt<std::string>(heap::stringkey, heap::stringsiz, siz * 2);
	nodes.resize(siz + 1);

	this->siz = siz;
	use       = 0;
}

heap::~heap(void)
{
	delete idmap;
}

int heap::insert(const std::string &id, int key, void *val)
{
	// heap filled to capacity
	if (use + 1 > siz) return 1;

	void *node = nullptr;

	// given id already exists
	if (!idmap->get(id, &node)) return 2;

	std::size_t pos = ++use;

	nodes[pos].id  = id;
	nodes[pos].key = key;
	nodes[pos].val = val;
	idmap->insert(id, &nodes[pos]);

	if (use == 1) return 0;

	percolate_up(pos);

	return 0;
}

int heap::setKey(const std::string &id, int key)
{
	node_t *p = nullptr;

	// given id doesn't exist
	if (idmap->get(id, (void**) &p) < 0) return 1;

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
	// heap is empty
	if (!use) return 1;

	if (id)  *id  = nodes[1].id;
	if (key) *key = nodes[1].key;
	if (val) *val = nodes[1].val;

	idmap->rm(nodes[1].id);
	nodes[1] = nodes[use--];

	if (use == 1) return 0;

	percolate_down(1);

	return 0;
}

int heap::remove(const std::string &id, int *key, void **val)
{
	node_t *p = nullptr;

	// given id doesn't exist
	if (idmap->get(id, (void**) &p) < 0) return 1;

	if (key) *key = p->key;
	if (val) *val = p->val;

	size_t pos = p - &nodes[0];

	idmap->rm(nodes[pos].id);
	nodes[pos] = nodes[use--];

	if (use == 1) return 0;

	if ((pos > 1) && (nodes[pos].key < nodes[pos >> 1].key))
		percolate_up(pos);
	else
		percolate_down(pos);

	return 0;
}