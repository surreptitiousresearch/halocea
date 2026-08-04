#pragma once
/* Node/bucket views over the opaque mapActors shim declared in DBG_CINE_INFO.h. The map ops
 * (Clear/Find/InsertNew/Begin/Next/Deref) walk the chained-hash table by hand, so they reinterpret
 * the shim's raw `data[20]` bucket vector and the `ITEM*` chain nodes through these DB-verified
 * views (the shim itself deliberately keeps them opaque). Layouts come straight from types_members
 * for the ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...> instantiation and its nested ITEM/dsVECTOR. */

#include "DBG_CINE_INFO.h" /* dsSTRID, DBG_CINE_ACTOR_INFO, DBG_CINE_MAP_ITER_T */

/* ds::MAP<...>::ITEM — chained-hash bucket node. DB-verified: next@0, key@4 (dsSTRID), value@8. */
typedef struct DBG_CINE_ACTOR_MAP_ITEM
{
    struct DBG_CINE_ACTOR_MAP_ITEM *next;  /* 0x00 next node in the same bucket chain */
    dsSTRID                         key;   /* 0x04 interned actor name */
    DBG_CINE_ACTOR_INFO             value; /* 0x08 per-actor capture record */
} DBG_CINE_ACTOR_MAP_ITEM;

/* dsVECTOR<ITEM*,8> bucket vector — the concrete view over the shim's `data[20]`.
 * DB-verified dsVECTOR layout: pData@0, nElem@4, allocated@8, __cl@12 (apCL) — size 20. */
typedef struct DBG_CINE_ACTOR_BUCKET_VEC
{
    DBG_CINE_ACTOR_MAP_ITEM **pData;     /* 0x00 heap array of bucket-chain heads */
    int                       nElem;     /* 0x04 bucket count */
    int                       allocated; /* 0x08 capacity */
    apCL                      __cl;      /* 0x0C allocation call-site cookie {file,line} */
} DBG_CINE_ACTOR_BUCKET_VEC;

/* Concrete view of the ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...> object behind the void* map handle.
 * Field offsets mirror DBG_CINE_INFO.h's MAP<...> shim (a@0, b@4, count@8, data@12, allocator@32,
 * cl@76); `data` is retyped here to the concrete bucket vector. */
typedef struct DBG_CINE_ACTOR_MAP_VIEW
{
    unsigned int              a;         /* 0x00 hash seed A */
    unsigned int              b;         /* 0x04 hash seed B */
    int                       count;     /* 0x08 live entry count */
    DBG_CINE_ACTOR_BUCKET_VEC data;      /* 0x0C bucket vector */
    /* 0x20 node pool. DB type dsNODE_CACHE_ALLOCATOR<...::DUMMY>; instantiated here with the
     * map's value type — layout-neutral, the allocator only stores node pointers. */
    dsNODE_CACHE_ALLOCATOR<DBG_CINE_ACTOR_INFO> allocator;
    apCL                      cl;        /* 0x4C */
} DBG_CINE_ACTOR_MAP_VIEW;
