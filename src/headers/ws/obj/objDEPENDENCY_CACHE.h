#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/MAP.h"
// ws-engine obj: cache of which objects depend on (share geometry from) which other
// objects. DB-verified layout (types_members objDEPENDENCY_CACHE) — size 88.

struct objOBJ; // ../obj/objOBJ.h — pointer only

// The hash-map machinery (ds::MAP, ds::impl::MAP_CONST_ITERATOR, dsNODE_CACHE_ALLOCATOR, ds::HASH,
// ds::CMP) is the canonical one from ../ds/MAP.h (included above) — do NOT redeclare it here or the
// template signatures conflict (MAP.h uses a `template<class> class A` allocator param).
namespace ds {
// ?AppendFast — bulk-append every element of `src` (from `start`, `count`=-1 meaning all) onto
// `dst`. boundary.
template<class T>
void AppendFast(dsVECTOR<T, 8> *dst, const dsVECTOR<T, 8> *src, int start, int count);
} // namespace ds

typedef struct objDEPENDENCY_CACHE {
    bool isCached;        // 0x00 set while the shared instance is (re)building the map
    bool hasSharinUsers;  // 0x01
    unsigned char _pad0[2]; // 0x02 db-verified padding
    // Hash map obj -> direct dependent users (84 bytes; see ../ds/MAP.h).
    ds::MAP<objOBJ *, dsVECTOR<objOBJ *,8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> dependencies; // 0x04

    // 0x82689D80 — append, transitively, every object that (directly or indirectly) shares
    // geometry from `obj` into `users`, following each newly-added user recursively.
    void GetAllDependentUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> *users) const;

    // 0x82689CC0 — append the direct sharing users of `obj` to `users`; returns how many
    // were added (0 if `obj` has no entry). Const (QBA); defers to the shared global instance
    // while a rebuild is in flight.
    int AddSharingUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> *users) const;

    // 0x8268B9B0 — (re)build the dependency map for `obj`'s scene tree: insert every node,
    // then register each geometry-sharing object as a user of its host.
    void Init(objOBJ *obj);
} objDEPENDENCY_CACHE;

// 0x842264C8 — the shared, lazily-built dependency cache instance.
extern objDEPENDENCY_CACHE objDependencyCache;
