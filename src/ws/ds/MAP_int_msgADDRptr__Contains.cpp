#include "MAP.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Contains @ 0x82559620
// True if `*key` is present; when `value` is non-null, copies the matched value out through it.
template<>
bool ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Contains(const int *key, msgADDR **value) const
{
    if (!data.nElem)
        return false;

    unsigned int hash = GetHash<int>(key);
    int bucket = (int)(hash % (unsigned int)data.nElem);

    if (!data[bucket])
        return false;

    ITEM *node = data[bucket];
    while (node && *key != node->key)
        node = node->next;

    if (!node)
        return false;

    if (value)
        *value = node->value;
    return true;
}
