#include "MAP.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Erase(const K*) @ 0x8255B6D8
// Remove every node in `key`'s bucket chain whose key equals `*key`. Walks with a pointer-to-the-
// link-that-references-the-current-node (`linkSlot`), which lets the unlink be a single pointer
// write; since ITEM::next is the FIRST member, a node's own address doubles as `&node->next`
// (matches the decompile's self-referential node-address-as-its-own-next-pointer idiom). Loop keeps
// going after each removal to catch any further matches later in the same chain, so this can remove
// more than one node even though Insert never creates duplicate keys in practice. Returns the
// removed count.
template<>
int ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Erase(const int *key)
{
    if (!data.nElem)
        return 0;

    int removed = 0;
    int nElem = data.nElem;
    unsigned int hash = GetHash<int>(key);
    int bucket = (int)(hash % (unsigned int)nElem);

    ITEM **linkSlot = &data[bucket];
    ITEM *node = data[bucket];
    for (;;) {
        while (node && *key != node->key) {
            linkSlot = &node->next;
            node = node->next;
        }
        if (!node)
            break;

        --count;
        ++removed;
        *linkSlot = node->next;

        // Return the unlinked node to the free list (tail-insert), matching the inline
        // PTR_LIST<DUMMY>::Insert-at-tail the decompile performs directly on allocator.freeNodeList.
        allocator.Delete(reinterpret_cast<dsMsgAddrMapDUMMY *>(node));

        node = *linkSlot;
    }
    return removed;
}
