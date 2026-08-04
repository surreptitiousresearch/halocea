#include "MAP.h"
#include "../ps/psSECTION.h"
#include "dsTSTRING_BUF_HEADER.h"
#include "dsTSTRING.h"

// ds::MAP<dsTSTRING<char>,psSECTION,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find(const K*)
// @ 0x82710EC8
// Locate `*key`'s bucket via GetHash<dsTSTRING<char>> % data.nElem, then walk the chain comparing
// keys: same shared-buffer pointer first (cheap common case for interned/aliased strings), else a
// length check followed by a byte-wise compare. Returns a MAP_ITERATOR whose `item` is null on a
// miss. DEVIATION: the decompile's per-byte loop computes a signed byte-difference `v18` that is
// only ever tested for zero/nonzero here (never its sign) — reproduced as a plain byte-equality
// scan, dropping the unused difference value. Field-offset mapping of the sret/this pair matches
// MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Find.cpp (decompiler swaps the sret iterator and the true
// `this` map pointer's names).
template<>
ds::impl::MAP_ITERATOR<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(const dsTSTRING<char> *key)
{
    ds::impl::MAP_ITERATOR<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;

    int nElem = this->data.nElem;
    if (nElem) {
        int bucketIndex = this->GetHash<dsTSTRING<char> >(key) % nElem;
        ITEM *node = this->data[bucketIndex];
        if (node) {
            dsTSTRING_BUF_HEADER<char> *keyBuffer = key->pBuffer;
            for (;;) {
                dsTSTRING_BUF_HEADER<char> *nodeBuffer = node->key.pBuffer;
                bool matches;
                if (nodeBuffer == keyBuffer) {
                    matches = true;
                } else if (nodeBuffer->strLen != keyBuffer->strLen) {
                    matches = false;
                } else if (nodeBuffer->strLen == 0) {
                    matches = true;
                } else {
                    matches = true;
                    const char *keyStr = keyBuffer->str;
                    const char *nodeStr = nodeBuffer->str;
                    const char *nodeStrEnd = nodeStr + nodeBuffer->strLen;
                    while (nodeStr != nodeStrEnd) {
                        if (*nodeStr != *keyStr) {
                            matches = false;
                            break;
                        }
                        ++nodeStr;
                        ++keyStr;
                    }
                }

                if (matches)
                    break;
                node = node->next;
                if (!node) {
                    result.container = nullptr;
                    result.item = nullptr;
                    result.index = 0;
                    return result;
                }
            }
            result.container = this;
            result.item = node;
            result.index = bucketIndex;
            return result;
        }
    }

    result.container = nullptr;
    result.item = nullptr;
    result.index = 0;
    return result;
}
