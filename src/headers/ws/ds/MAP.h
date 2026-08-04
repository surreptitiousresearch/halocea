#pragma once
#include "CMP.h"
#include "dsVECTOR.h"
#include "dsNODE_CACHE.h"
#include "dsNODE_CACHE_ALLOCATOR.h"
#include "NAMED_PAIR.h"
#include "BEGIN_HELPER.h"
#include "../../apCL.h"
// ws-engine ds: chained-hash map, keyed by K -> value V, parameterised by a hashing policy H, an
// equality policy C, and a node-pool allocator A. DB-verified layout (types_members ds::MAP<...>):
//   a@0, b@4, count@8, data@12 (bucket vector), allocator@32 (dsNODE_CACHE_ALLOCATOR pool), cl@76
//   — size 84. `a`/`b` are the two randomized seeds ds::HASH::hash<K> mixes into every hash
//   (reseeded by GenHashes); the bucket index is always `GetHash(key) % data.nElem` (recomputed
//   live, never cached in the node). CORRECTED (msgADDR-map batch, DB-verified via the reversed
//   MAP<int,msgADDR*,...>::~MAP and ::Insert bodies): `allocator`'s real type is
//   `dsNODE_CACHE_ALLOCATOR<DUMMY>`, which PUBLICLY DERIVES FROM `dsNODE_CACHE<DUMMY>` (same 44
//   bytes, adds no fields, just renames GetEmpty/Release to Alloc/Delete) — not `dsNODE_CACHE<DUMMY>`
//   directly as an earlier pass on a sibling instantiation had it; the dtor decompile explicitly
//   casts `&this->allocator` to `dsNODE_CACHE_ALLOCATOR<DUMMY> *`. This is a safe widening (every
//   dsNODE_CACHE<DUMMY> method remains callable on it via inheritance).

namespace ds {
    // ds::HASH — stateless hashing policy (DB: no data members). hash<K> is a boundary primitive
    // (the concrete mixing function lives outside ds_map.hpp, in ds::HASH's own translation unit);
    // ds::MAP only ever calls it via GetHash<K>, which additionally range-asserts the result
    // against ds::BIG_PRIME.
    struct HASH {
        template<class K>
        static unsigned int hash(const K &key, unsigned int seedA, unsigned int seedB); // boundary
    };
    // ds::CMP — stateless equality policy (DB: no data members); ds::MAP<int,...> compares keys
    // with `==` directly rather than going through CMP, so CMP is unused by the reversed methods.

    // boundary primitives from ds_map.hpp (prime-table helpers backing GenHashes/HashResize).
    extern const unsigned int BIG_PRIME;         // upper bound asserted on every GetHash result
    bool CheckPrime(int n);                      // true if `n` is already usable as a bucket count
    int  FindPrime(int n);                       // next usable bucket-count prime >= n
}

namespace ds {
// Forward declaration so the iterator ctors below (which take a MAP& before MAP itself is
// defined further down this file) can name it.
template<class K, class V, class H, class C, template<class> class A>
struct MAP;
}

namespace ds { namespace impl {

// Const-position handle over a ds::MAP. DB-verified layout: container@0, item@4, index@8.
// dscBRAND::Init (0x82711B28) calls this base's operator* directly on a MAP_ITERATOR object
// returned by ds::MAP::Find, confirming MAP_ITERATOR publicly derives from this type.
template<class K, class V, class H, class C, template<class> class A>
struct MAP_CONST_ITERATOR {
    void *container; // 0x00 owning ds::MAP
    void *item;      // 0x04 matched node (null once the walk is done / key absent)
    int   index;     // 0x08 node index within the map

    // 0x82681080 (MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — null/zero every field.
    MAP_CONST_ITERATOR();
    // 0x826810B8 (MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — position at `item`
    // (`container`/bucket `index` supplied by the caller, e.g. MAP::Find/Begin).
    MAP_CONST_ITERATOR(MAP<K, V, H, C, A> *container, typename MAP<K, V, H, C, A>::ITEM *item, int index);

    const V &operator*() const;  // boundary
    const V *operator->() const; // boundary

    // true once `item` is null (the walk ran off the end / the key was absent). Reversed
    // per-instantiation (asserts nothing itself; distinct from the STRONG_ASSERT'ing key()/value()).
    bool IsDone() const;
    // Reference to the matched node's key; STRONG_ASSERT(item != 0) first (d:\...\ds_map.hpp:158).
    // Reversed per-instantiation.
    const K &key() const;
    // Reference to the matched node's value; STRONG_ASSERT(item != 0) first (ds_map.hpp:169).
    // Reversed per-instantiation.
    const V &value() const;

    // 0x82686310 (MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — advance to the next
    // chained node in the current bucket, else the next non-empty bucket; STRONG_ASSERT-crashes if
    // already done or the map is empty. Reversed per-instantiation.
    MAP_CONST_ITERATOR &operator++();
};

// Mutable position handle over a ds::MAP. Adds no new fields over MAP_CONST_ITERATOR.
template<class K, class V, class H, class C, template<class> class A>
struct MAP_ITERATOR : MAP_CONST_ITERATOR<K, V, H, C, A> {
    // 0x82684420 (MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — null/zero every field.
    MAP_ITERATOR();
    // 0x82684438 (MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — position at `item`.
    MAP_ITERATOR(MAP<K, V, H, C, A> *container, typename MAP<K, V, H, C, A>::ITEM *item, int index);
    // 0x82680B10 (MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — unwrap the BEGIN_HELPER Begin()
    // returns (bytewise copy of its `iter`).
    MAP_ITERATOR(const ds::impl::BEGIN_HELPER<MAP_ITERATOR> &helper);

    // true once the iterator has walked past the last matching node (item == null). boundary
    bool IsDone() const;  // boundary
    // 0x825575B8 (MAP_ITERATOR<int,msgADDR*,...>) — pointer to the matched value slot; strong-
    // asserts `item != 0` first (ds_map.hpp:169). Reversed per-instantiation.
    V *value() const;

    // 0x826842D0 (MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — thunk: forwards to the base
    // MAP_CONST_ITERATOR::operator-> (same STRONG_ASSERT + `&item->value`), reinterpreted mutable.
    V *operator->() const;
    // 0x82687CC8 (MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — own copy of the base's
    // operator++ walk (chain-then-bucket advance); identical body to MAP_CONST_ITERATOR's.
    MAP_ITERATOR &operator++();
};

} } // namespace ds::impl

namespace ds {

template<class K, class V, class H, class C, template<class> class A>
struct MAP {
    // Chained hash-bucket node. DB-verified layout: next@0, key@4, value@8.
    struct ITEM {
        ITEM *next;  // 0x00 next node in the same bucket chain
        K     key;   // 0x04
        V     value; // 0x08

        // 0x8268A288 (ITEM<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — copy-construct: next=null,
        // key=*key, value default-constructed then bulk-inserted with `*value`'s elements (the
        // dsVECTOR<K,8>::Insert(0, pData, nElem) idiom, not a plain operator= — matches the
        // disassembly exactly; equivalent to a copy but goes through Insert). Reversed
        // per-instantiation.
        ITEM(const K *key, const V *value);
        // 0x82684448 (ITEM<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — free `value`'s backing store
        // (pointer/POD `key` needs no teardown). Reversed per-instantiation.
        ~ITEM();
        // 0x82684BD0 (ITEM<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — compiler-generated deleting-dtor
        // entry point: runs ~ITEM(), then conditionally operator delete(this) when bit 0 of the
        // flag argument is set. Reversed per-instantiation.
        ITEM *scalarDeletingDtor(unsigned char deleteFlags);
    };

    // Same-size placeholder node type used by the node-pool allocator's own bookkeeping (nodeList
    // / freeNodeList are typed `DUMMY *`, not `ITEM *` — the sibling dsNODE_CACHE<DUMMY> methods
    // never dereference a node's payload, only link/unlink it, so its true layout is irrelevant
    // and left opaque). Per-instantiation `dsCineActorMapDUMMY`-style opaque tags are the same type.
    //
    // NOTE ON dsVECTOR<ITEM*,8>/dsVECTOR<DUMMY*,8> SPECIALIZATIONS: this class's `data`/`allocator`
    // members are declared here with the real nested `ITEM`/`DUMMY` types (so the template stays
    // generic across K,V instantiations), but the low-level dsVECTOR<T*,8>/dsNODE_CACHE<DUMMY>
    // per-instantiation method bodies (Realloc/operator[]/GetEmpty/etc.) are written against a
    // separately-declared, layout-identical opaque tag (e.g. `dsObjOBJVecMapITEM`/
    // `dsObjOBJVecMapDUMMY` for the objOBJ*->dsVECTOR<objOBJ*,8> instantiation) rather than the
    // nested type name, matching the convention already established across every MAP<...>
    // instantiation reversed this session (msgADDR-map, dsSTRID-map, CineActor-map, objOBJ-map).
    // The two are ABI/layout-identical; treat them as interchangeable when reading across files.
    struct DUMMY;

    unsigned int          a;        // 0x00 FindPrime-derived seed (ctor: FindPrime(RndInt()+10))
    unsigned int          b;        // 0x04 random salt (ctor: RndInt())
    int                   count;    // 0x08 number of live entries
    dsVECTOR<ITEM *, 8>   data;     // 0x0C hash buckets (chained ITEM lists)
    dsNODE_CACHE_ALLOCATOR<DUMMY> allocator; // 0x20 node pool (44B; derives dsNODE_CACHE<DUMMY>)
    apCL                  cl;       // 0x4C allocation call-site cookie

    // 0x8255C2D8 (MAP<int,msgADDR*,...>) — zero `count`/`data`, default-construct `allocator`
    // (which zeroes itself and stamps its own __cl from `cl`), copy `cl` into this->cl, then call
    // GenHashes to seed a/b. Deviation: the raw decompile ("local variable allocation has failed")
    // additionally shows two corrupted stores into what would be `data`'s trailing apCL bytes
    // (`data.__cl.file = this`, `data.__cl.line = &cl`) — nonsensical for an apCL value and dropped
    // as reg-alloc-failure noise; `data`'s own apCL is left default (zero), matching dsVECTOR's
    // documented default ctor. Reversed per-instantiation. CAVEAT.
    MAP(apCL cl);
    // 0x8255CBB8 (MAP<int,msgADDR*,...>) — Clear() the buckets, Term() the allocator, then destroy
    // nodeList/data. Reversed per-instantiation.
    ~MAP();

    // 0x82559468 (MAP<int,msgADDR*,...>) — look up `*key`; the returned iterator's `item` is null
    // when absent or the table has no buckets yet. Reversed per-instantiation.
    ds::impl::MAP_ITERATOR<K, V, H, C, A> Find(const K *key);
    // 0x82687C78 (MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — const overload: delegates to the mutable
    // Find above and copies the result into a MAP_CONST_ITERATOR (identical field layout). Reversed
    // per-instantiation.
    ds::impl::MAP_CONST_ITERATOR<K, V, H, C, A> Find(const K *key) const;
    // 0x8255D078 (MAP<int,msgADDR*,...>) — insert-or-replace `*key` -> `*value`. sret/this-swap
    // (per corpus convention): the FIRST parameter below is the hidden sret buffer for the returned
    // NAMED_PAIR and the SECOND is the real `this`, matching the reversed .cpp's parameter order.
    // On a key match, overwrites the existing value in place (wasReplaced=true, iterator at the
    // matched node). On a miss, allocates a node from `allocator`, prepends it after the last node
    // of the bucket chain — then, because the insert may have crossed a resize threshold,
    // re-resolves the just-linked node's bucket/iterator AFTER calling TryToResize (bucket count
    // and index can change) rather than reusing the pre-resize index (wasReplaced=false). Reversed
    // per-instantiation.
    ds::NAMED_PAIR<ds::NAMED_PAIR_value<ds::impl::MAP_ITERATOR<K, V, H, C, A> >, ds::NAMED_PAIR_wasReplaced<bool> >
        Insert(const K *key, V *value);

    // Compute the raw (unreduced-by-bucket-count) hash of `*key` using this map's per-instance
    // salt fields (a = odd bucket-count-ish prime seed from FindPrime, b = random salt), both set
    // by the ctor. Callers (Find/Insert) further reduce the result modulo `data.nElem`. Reversed
    // per-instantiation (cf. ds::HASH::hash<dsTSTRING<char>>); sibling ds containers declare the
    // same shape (dsLF_SORTED_MAP::GetHash, objDEPENDENCY_CACHE::GetHash). Return type corrected to
    // `unsigned int` (0x82558E90 returns the raw ds::HASH::hash<K> result, strong-asserted against
    // ds::BIG_PRIME — not a signed reduced index).
    template<class KK>
    unsigned int GetHash(const KK *key) const;

    // 0x823B9FD8 (MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>) — find-or-insert: Find(key); if found,
    // return that iterator (this=result's `this` swap mirrors the corpus sret/this-swap
    // convention: the raw decompile stores the found iterator's fields back onto `this` before
    // returning it). On a miss, InsertNew(key, *value) and return its iterator. Reversed
    // per-instantiation.
    ds::impl::MAP_ITERATOR<K, V, H, C, A> FindInsert(const K *key, const V *value);

    // 0x823B9EFC (MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>) — private helper behind FindInsert's miss
    // path: HashResize(17) if the table has no buckets yet, compute the new node's bucket
    // (GetHash(key) % data.nElem), bump `count`, then Insert(&bucketHead, *key, *value) (the
    // 3-arg bucket-prepend overload) and return an iterator at the just-inserted node. Reversed
    // per-instantiation.
    ds::impl::MAP_ITERATOR<K, V, H, C, A> InsertNew(const K *key, const V *value);

    // 0x8255CC10 (MAP<int,msgADDR*,...>) — private helper: allocate a fresh node from `allocator`,
    // populate it with (key,*value), and PREPEND it onto the intrusive bucket chain whose head
    // pointer is `*bucketHead` (the caller has already located the correct bucket slot). Distinct
    // from the public 2-arg Insert above. Reversed per-instantiation.
    void Insert(ITEM **bucketHead, const K &key, const V &value);

    // 0x82711BAC — remove the entry at `iter`. `iter` is passed BY VALUE in registers (the
    // small POD MAP_ITERATOR has no user-defined copy ctor/dtor, unlike psSECTION's non-trivial
    // by-value convention). Returns the resulting iterator position; body not decompiled here.
    ds::impl::MAP_ITERATOR<K, V, H, C, A> Erase(ds::impl::MAP_ITERATOR<K, V, H, C, A> iter); // boundary
    // 0x8255B6D8 (MAP<int,msgADDR*,...>) — remove every node whose key equals `*key` (bucket-chain
    // walk that keeps unlinking while it keeps matching, stopping at the first non-match). Returns
    // the number of nodes removed (0 or 1 in practice, since Insert never creates duplicate keys).
    // A second, key-based overload alongside the iterator-based Erase above. Reversed
    // per-instantiation.
    int Erase(const K *key);
    // 0x8255B888 (MAP<int,msgADDR*,...>) — return every live node to the free list and reset all
    // bucket heads to null and count to 0 (bucket capacity/`data.nElem` is left unchanged — buckets
    // are reused, not shrunk). Reversed per-instantiation.
    void Clear();
    // 0x82559620 (MAP<int,msgADDR*,...>) — true if `*key` is present; when `value` is non-null,
    // also copies out the matched value. Reversed per-instantiation.
    bool Contains(const K *key, V *value) const;
    // 0x8255AD20 (MAP<int,msgADDR*,...>) — return `ptr` to the allocator's free list directly
    // (bypasses bucket unlinking; for callers that have already unlinked the node themselves).
    // Reversed per-instantiation.
    void Delete(DUMMY *ptr);
    // 0x82556670 (MAP<int,msgADDR*,...>) — reseed the hash (a = FindPrime(rand()+10), b = rand())
    // via the shared m3dRND_GEN generator; called by the ctor and by TryToResize once resize depth
    // exceeds 10. Reversed per-instantiation.
    void GenHashes();
    // 0x8255C390 (MAP<int,msgADDR*,...>) — rehash every live node into a freshly-sized `size`-bucket
    // (prime-rounded, bumped again if it would collide with the current seed `a`) vector, then swap
    // it in for `data`. Reversed per-instantiation.
    void HashResize(int size);
    // 0x8255C590 (MAP<int,msgADDR*,...>) — grow-on-demand policy: no buckets yet -> HashResize(17);
    // else resize (to double the bucket count) once the chain-walk `depth` or the global load
    // factor crosses escalating thresholds (count > 5x buckets always, > 3x past depth 10, > 2x
    // past depth 25), reseeding first once depth > 10. Returns whether a resize occurred (so Insert
    // knows to re-resolve its just-inserted node's iterator). Reversed per-instantiation.
    bool TryToResize(int depth);

    // 0x82686130 (MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>) — scan forward from bucket 0 for the first
    // non-null bucket head; wraps the resulting MAP_ITERATOR (or the empty/done iterator, if no
    // bucket has an entry) in a BEGIN_HELPER for MAP_ITERATOR's ctor to unwrap. Reversed
    // per-instantiation.
    ds::impl::BEGIN_HELPER<ds::impl::MAP_ITERATOR<K, V, H, C, A> > Begin();
};

} // namespace ds
