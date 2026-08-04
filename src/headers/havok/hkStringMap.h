#pragma once

/* hkStringMapOperations — stateless key-ops policy (DB size 1, no members). */
typedef struct hkStringMapOperations
{
} hkStringMapOperations;

/* Generic hkCachedHashMap<OPS> — open hash map with cached ops policy.
   DB instantiation hkCachedHashMap<hkStringMapOperations> (16 bytes):
   m_elem@0, m_numElems@4, m_hashMod@8, m_ops@12. */
template<class OPS>
struct hkCachedHashMap
{
    unsigned int *m_elem; /* 0x00 */
    int m_numElems;       /* 0x04 */
    int m_hashMod;        /* 0x08 */
    OPS m_ops;            /* 0x0C */
};

/* types_members hkStringMapBase (16 bytes) — just the cached-hash-map base. */
struct hkStringMapBase : hkCachedHashMap<hkStringMapOperations>
{
};
typedef struct hkStringMapBase hkStringMapBase;

/* hkStringMap<T> wraps a single hkStringMapBase at offset 0 (generic template;
   the DB stores only instantiations). */
template<class T = void>
struct hkStringMap_t
{
    hkStringMapBase m_map;
};
typedef hkStringMap_t<> hkStringMap;

/* hkCachedHashMap<hkStringMapOperations>::getWithDefault -- the string pointer is
   passed directly as the integer key. Boundary. */
void *hkStringMap_getWithDefault(const hkStringMapBase *map, unsigned int key, void *defaultValue);
