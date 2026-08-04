#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(const K*,V*)
// @ 0x8268B7B8
// Public insert-or-replace: presize the bucket vector to 17 on first use, then locate `*key`'s
// bucket. On a match, overwrite the existing node's value in place (wasReplaced=true, iterator at
// the matched node). On a miss, splice a fresh node in via the private Insert(ITEM**,K&,V&) helper,
// bump `count`, then TryToResize — which may rehash and move the just-inserted node to a different
// bucket, so the returned iterator's bucket/node pointer is re-resolved AFTER the resize check
// rather than reusing the pre-resize position (wasReplaced=false either way).
template<>
ds::NAMED_PAIR<
    ds::NAMED_PAIR_value<ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> >,
    ds::NAMED_PAIR_wasReplaced<bool> >
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(
    objOBJ *const *key, dsVECTOR<objOBJ *, 8> *value)
{
    typedef ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ResultIter;

    if (!this->data.nElem) {
        ITEM *nullFill = nullptr;
        this->data.Resize(17, nullFill);
    }

    int nElem = this->data.nElem;
    int bucketIndex = this->GetHash<objOBJ *>(key) % nElem;
    ITEM *bucketHead = this->data[bucketIndex];

    if (bucketHead) {
        // Walk the chain looking for an existing node with this key (chainDepth counts the walk
        // for TryToResize's escalation heuristic on the miss path below).
        int chainDepth = 1;
        ITEM *prev = bucketHead;
        ITEM *node = bucketHead->next;
        while (node && prev->key != *key) {
            prev = node;
            node = node->next;
            ++chainDepth;
        }

        if (prev->key == *key) {
            prev->value = *value;
            ResultIter iter;
            iter.container = this;
            iter.item = prev;
            iter.index = bucketIndex;
            return ds::NAMED_PAIR<ds::NAMED_PAIR_value<ResultIter>, ds::NAMED_PAIR_wasReplaced<bool> >(iter, true);
        }

        ++this->count;
        this->Insert(&prev->next, *key, *value);
        ITEM *inserted = prev->next;

        if (this->TryToResize(chainDepth)) {
            int newBucket = this->GetHash<objOBJ *>(key) % this->data.nElem;
            ResultIter iter;
            iter.container = this;
            iter.item = inserted;
            iter.index = newBucket;
            return ds::NAMED_PAIR<ds::NAMED_PAIR_value<ResultIter>, ds::NAMED_PAIR_wasReplaced<bool> >(iter, false);
        }

        ResultIter iter;
        iter.container = this;
        iter.item = inserted;
        iter.index = bucketIndex;
        return ds::NAMED_PAIR<ds::NAMED_PAIR_value<ResultIter>, ds::NAMED_PAIR_wasReplaced<bool> >(iter, false);
    }

    // Empty bucket: splice a fresh node directly as the bucket head. DEVIATION: the raw decompile
    // inlines the private Insert(ITEM**,K&,V&) helper's body here instead of calling it (the
    // non-empty-bucket miss path above DOES show a real call to the same helper) — almost certainly
    // the same source call site with this instance inlined by the optimizer; reconstructed as the
    // single logical call for source fidelity, with no behavioral difference (does NOT call
    // TryToResize on this path, matching the decompile).
    ++this->count;
    ITEM **bucketHeadSlot = &this->data[bucketIndex];
    this->Insert(bucketHeadSlot, *key, *value);

    ResultIter iter;
    iter.container = this;
    iter.item = this->data[bucketIndex];
    iter.index = bucketIndex;
    return ds::NAMED_PAIR<ds::NAMED_PAIR_value<ResultIter>, ds::NAMED_PAIR_wasReplaced<bool> >(iter, false);
}
