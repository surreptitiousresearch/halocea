#pragma once
/* DBG_CINE_INFO — HCEX debug cinematic-dump record (the global _dbgCineInfo). Tracks the path/name of the
 * cinematic currently being captured, whether capture is inited, the elapsed time/frame, and a map of the
 * per-actor capture state. Layout verified via types_members DBG_CINE_INFO (size 104).
 * mapActors is a ws-engine ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>;
 * a local boundary MAP shim reproduces its DB-verified 84-byte layout (a@0, b@4, count@8, data@12,
 * allocator@32, cl@76), and its lifecycle ops are declared as free functions (the compiler-generated
 * thiscall). dsTSTRING<char> here is the flattened boundary spelling of dsTSTRING<char>. */

#include "hcex_ds_boundary.h"
#include "../apCL.h"

/* dsVECTOR<T*,8> — flattened boundary shim of the pointer-vector instantiations the map uses
 * (DB-verified dsVECTOR layout: pData@0, nElem@4, allocated@8, __cl@12 (apCL) — size 20;
 * canonical C++ form src/headers/ws/ds/dsVECTOR.h). */
template<class T>
struct DBG_CINE_PTR_VEC
{
    T           **pData;     /* 0x00 heap array of node pointers */
    int           nElem;     /* 0x04 live count */
    int           allocated; /* 0x08 capacity */
    apCL          __cl;      /* 0x0C allocation call-site cookie */
};

/* ds::PTR_LIST<T,ds::LIST_NODE_ACCESS_POLICY<T>> — flattened boundary shim (DB-verified layout:
 * pHead@0, pTail@4, length@8 — size 12; canonical C++ form src/headers/ws/ds/PTR_LIST.h). */
template<class T>
struct DBG_CINE_PTR_LIST
{
    T   *pHead;  /* 0x00 */
    T   *pTail;  /* 0x04 */
    int  length; /* 0x08 */
};

/* ds::HASH — stateless hashing policy (types_members ds::HASH — no data members). */
typedef struct HASH
{
} HASH;

/* ds::CMP — stateless equality policy (types_members ds::CMP — no data members). */
typedef struct CMP
{
} CMP;

/* dsNODE_CACHE_ALLOCATOR<DUMMY> — 44-byte node-pool allocator; generic boundary shim
 * (the DB stores only instantiations). Layout filled from types_members
 * dsNODE_CACHE_ALLOCATOR<ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>::DUMMY> (2026-08-04):
 * a single unnamed dsNODE_CACHE<DUMMY> base of the same 44 bytes — nNode@0, nodeList@4
 * (dsVECTOR<DUMMY*,8>, 20), freeNodeList@24 (ds::PTR_LIST<DUMMY,...>, 12), __cl@36 (apCL, 8).
 * Inner containers typed via the flattened DBG_CINE_PTR_VEC/DBG_CINE_PTR_LIST shims above
 * (byteshim reconciliation 2026-08-04, offsets unchanged); the fully reversed form is
 * src/headers/ws/ds/dsNODE_CACHE_ALLOCATOR.h. */
template<class DUMMY>
struct dsNODE_CACHE_ALLOCATOR
{
    int                       nNode;        /* 0x00 live node count */
    DBG_CINE_PTR_VEC<DUMMY>   nodeList;     /* 0x04 dsVECTOR<DUMMY*,8> node pool */
    DBG_CINE_PTR_LIST<DUMMY>  freeNodeList; /* 0x18 ds::PTR_LIST<DUMMY,...> free list */
    apCL                      cl;           /* 0x24 */
};

/* ds::MAP<K,V,H,C,A> — 84-byte chained-hash map, boundary shim of the DB-verified layout
 * (see src/headers/ws/ds/MAP.h for the fully reversed C++ form). */
template<class K, class V, class H, class C, class A>
struct MAP
{
    /* Chained hash-bucket node (next@0, key@4, value@8); left incomplete here — the concrete
     * view is DBG_CINE_ACTOR_MAP_ITEM in DBG_CINE_ACTOR_MAP_NODE.h. */
    struct ITEM;

    unsigned int           a;         /* 0x00 hash seed A */
    unsigned int           b;         /* 0x04 hash seed B */
    int                    count;     /* 0x08 */
    DBG_CINE_PTR_VEC<ITEM> data;      /* 0x0C dsVECTOR<ITEM*,8> bucket vector */
    A                      allocator; /* 0x20 dsNODE_CACHE_ALLOCATOR<...> pool (DB tag: <DUMMY>;
                                       * pointer-only storage, so the arg is layout-neutral) */
    apCL                   cl;        /* 0x4C */
};

/* DBG_CINE_ACTOR_INFO — per-actor debug record stored in mapActors (keyed by dsSTRID). Flattened plain-C
 * mirror of the real ws-engine C++ type (src/headers/ws/dbg/DBG_CINE_ACTOR_INFO.h), for use from the
 * hcex bridge layer. DB-verified layout: filePath@0 (dsTSTRING<char>), wasUpdatedCurFrame@4 (bool). */
typedef struct DBG_CINE_ACTOR_INFO
{
    dsTSTRING<char> filePath;       /* 0x00 capture output path for this actor */
    bool wasUpdatedCurFrame;  /* 0x04 set once refreshed this frame */
} DBG_CINE_ACTOR_INFO;

typedef struct DBG_CINE_INFO
{
    dsTSTRING<char>     path;             /* 0x00 */
    dsTSTRING<char>     name;             /* 0x04 */
    bool          isInited;         /* 0x08 */
    unsigned char _pad09[3];        /* 0x09 */
    float         timeFromStart;    /* 0x0C */
    int           frame;            /* 0x10 */
    /* A param spelled with an explicit arg (layout-neutral: the allocator only stores node
     * pointers); a bare class template can't be a template argument in strict C++. */
    MAP<dsSTRID, DBG_CINE_ACTOR_INFO, HASH, CMP, dsNODE_CACHE_ALLOCATOR<DBG_CINE_ACTOR_INFO> > mapActors; /* 0x14 */
} DBG_CINE_INFO;

/* ds::MAP<...>::Clear on the mapActors sub-object */
extern void DBG_CINE_INFO_mapActors_Clear(void *map_actors);

/* DBG_CINE_INFO ctor / operator= / dtor (ws-engine compiler-generated thiscall form) */
extern void DBG_CINE_INFO_ctor(DBG_CINE_INFO *self);
extern DBG_CINE_INFO *DBG_CINE_INFO_assign(DBG_CINE_INFO *self, const DBG_CINE_INFO *other); // operator= returns *this
extern void DBG_CINE_INFO_dtor(DBG_CINE_INFO *self);

/* --- ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR> ops on mapActors ---
 * A position handle over the map: `container` is the owning map (opaque), `item` is the matched bucket
 * node (opaque; null once absent/done), `index` is the bucket index. Mirrors
 * ds::impl::MAP_(CONST_)ITERATOR<...>'s DB-verified layout (container@0, item@4, index@8); the DB holds
 * no iterator instantiation for this V-type, so the shim stays generic. */
template<class K, class V>
struct DBG_CINE_MAP_ITER_T
{
    void *container; /* 0x00 owning ds::MAP<K,V,...> */
    void *item;      /* 0x04 matched ITEM* node (null when absent/done) */
    int   index;     /* 0x08 bucket index */
};
typedef DBG_CINE_MAP_ITER_T<dsSTRID, DBG_CINE_ACTOR_INFO> DBG_CINE_ACTOR_MAP_ITER;

/* DBG_CINE_ACTOR_INFO::DBG_CINE_ACTOR_INFO(const dsTSTRING<char>&, float) */
extern void DBG_CINE_ACTOR_INFO_ctor(DBG_CINE_ACTOR_INFO *self, const dsTSTRING<char> *path, float startTime);
/* DBG_CINE_ACTOR_INFO::DumpMatrix(const m3dMATR&) */
extern void DBG_CINE_ACTOR_INFO_DumpMatrix(DBG_CINE_ACTOR_INFO *self, const void *matrL2W);

/* ds::MAP<...>::Find(&key) */
extern DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Find(void *map_actors, const dsSTRID *key);
/* ds::MAP<...>::FindInsert(&key, &value) — find-or-insert */
extern DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_FindInsert(void *map_actors, const dsSTRID *key,
                                                                    const DBG_CINE_ACTOR_INFO *value);
/* ds::MAP<...>::GetHash<dsSTRID>(&key) */
extern unsigned int DBG_CINE_INFO_mapActors_GetHash(void *map_actors, const dsSTRID *key);
/* ds::MAP<...>::InsertNew(&key, &value) — unconditional insert (caller has already verified the
 * key is absent, e.g. via FindInsert's own Find probe) */
extern DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_InsertNew(void *map_actors, const dsSTRID *key,
                                                                    const DBG_CINE_ACTOR_INFO *value);
/* ds::MAP<...>::Begin() */
extern DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Begin(void *map_actors);
/* ds::impl::MAP_ITERATOR<...>::operator++() */
extern DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Next(DBG_CINE_ACTOR_MAP_ITER iter);
/* ds::impl::MAP_(CONST_)ITERATOR<...>::operator->() — asserts iter.item != null */
extern DBG_CINE_ACTOR_INFO *DBG_CINE_INFO_mapActors_Deref(DBG_CINE_ACTOR_MAP_ITER iter);
