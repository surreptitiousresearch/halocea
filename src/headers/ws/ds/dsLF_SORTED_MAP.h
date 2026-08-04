#pragma once
#include "../../apCL.h"
#include "dsVECTOR.h"
#include "dsERASE_MANAGER.h"

// ws-engine ds: lock-free sorted hash map keyed on K with value V, using HASH for bucket
// placement and CMP for ordering. Buckets are singly-linked LINE nodes held in a growable
// hash table; deletions are deferred through a per-map dsERASE_MANAGER.
// DB-verified layout (types_members
//   dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>):
//   a@0, b@4, hashTable@8 (dsVECTOR<LINE*,8>, 20 bytes), cl@28 (apCL).

template<class K, class V, class HASH, class CMP>
struct dsLF_SORTED_MAP {
    // A single key/value slot inside a chain node — DB-verified
    // (dsLF_SORTED_MAP<char const*,int,...>::ITEM): key@0 (K), val@4 (V).
    struct ITEM {
        K key; // 0x00
        V val; // 0x04
    };

    // A hash-chain node: an ascending-sorted (by key) run of ITEMs — DB-verified
    // (dsLF_SORTED_MAP<char const*,int,...>::LINE): length@0 (int), items@4 (ITEM[1], flexible).
    struct LINE {
        int  length;   // 0x00 number of live items in this node
        ITEM items[1];  // 0x04 flexible array of sorted items
    };

    // Iterator/eraser cursor over the chain — DB-verified
    // (dsLF_SORTED_MAP<...>::LINE_ERASER): line@0 (LINE*), index@4 (int).
    struct LINE_ERASER {
        LINE *line;  // 0x00 current chain node
        int   index; // 0x04 slot index within the node
    };

    unsigned int         a;         // 0x00 lock-free version/generation word
    unsigned int         b;         // 0x04 lock-free version/generation word
    dsVECTOR<LINE *, 8>  hashTable; // 0x08 bucket array (20 bytes)
    apCL                 cl;        // 0x1C allocation call-site cookie

    // 0x825291F0 (dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>) — construct with the
    // given allocation call-site cookie: adopts `cl` for the bucket-array dsVECTOR and for its own
    // `cl`, then seeds the a/b hash salt pair (a = FindPrime(RndInt()+10), b = RndInt()). Sibling of
    // dsSTRID_POOL's ctor, which calls it as its stringMap member-initializer.
    dsLF_SORTED_MAP(apCL cl);

    // 0x82529F48 / 0x826164C0 — process-wide deferred-erase manager for this map type
    // (function-local Meyers singleton). Static: shared by all instances of the type.
    static dsERASE_MANAGER<LINE_ERASER> *GetEraseMgr();

    // 0x825285C8 / 0x82613D18 — binary search of a single chain node for `key`; returns the
    // item index within the node, or -1 if absent. const method (ABA mangle).
    int FindKey(const LINE *line, const K *key) const;

    // 0x82528648 / 0x82613D98 — lower-bound search of a single chain node: return the index of
    // the first item whose key is >= `key`. Returns 0 for a null/empty node. When `key` matches
    // an item, scans left to return the FIRST equal item. const method (ABA mangle).
    int FindKeyGE(const LINE *line, const K *key) const;

    // Compute the (unmasked) bucket hash for `key`; body not decompiled here. const.
    int GetHash(const K *key) const; // boundary

    // 0x8252A488 — look up `key`; returns a pointer to the stored value, or null if absent. const.
    const V *Find(const K *key) const;

    // 0x8252B5D0 / 0x8261A8C0 — insert `key`->`val` into its hash bucket, keeping the bucket chain
    // key-sorted. Registers the thread with the deferred-erase manager, locates the lower bound in
    // the target node, asserts the key is not already present, then splices a new item and publishes
    // the (possibly reallocated) node back into the bucket behind a memory barrier.
    // Reversed per-instantiation.
    void Insert(const K &key, const V &val);

    // 0x8252B460 / 0x8261A6E8 — build a fresh chain node holding `line`'s items with `item`
    // spliced in at `index` (copying the [0,index) prefix and the [index,length) suffix around
    // the gap), queue the old node for deferred destruction via the erase manager, and return the
    // new node. Reversed per-instantiation.
    LINE *InsertItem(LINE *line, int index, const ITEM *item);
};
