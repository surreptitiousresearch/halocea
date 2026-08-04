#pragma once

#include "hkBool.h"
#include "hkReferencedObject.h"

/* hkaChunkCache — abstract decompression-chunk cache used by the wavelet/delta
   animation decompressors. Keys map to fixed-size chunks; the cache serialises
   access with an internal critical section and per-key read/write locks. Layout
   and vtable verified against the DB (types_members: hkaChunkCache,
   hkaChunkCache_vtbl). */

typedef struct hkaChunkCache hkaChunkCache;
typedef struct hkaChunkCache_vtbl hkaChunkCache_vtbl;

/* Forward decls for slots that are not exercised by the callers reversed here. */
struct hkClass;
struct hkStatisticsCollector;
struct hkOstream;

/* NOTE ON THE hkBool-RETURNING SLOTS (flushKey/…/isKeyLockedForWrite/printCacheStats):
   the DB lists them as (this, hkBool *result, ...) — the PPC struct-return-by-value
   ABI hidden result pointer. Call sites pass (result, this, ...) in register order;
   they are modelled here in true register order (result, this, ...) to match the call sites. */
struct hkaChunkCache_vtbl
{
    void (*dtr_hkaChunkCache)(hkaChunkCache *self);                                                              /* +0  */
    const struct hkClass *(*getClassType)(hkaChunkCache *self);                                                  /* +4  */
    void (*calcContentStatistics)(hkaChunkCache *self, struct hkStatisticsCollector *, const struct hkClass *);  /* +8  */
    const unsigned char *(*retrieveChunk)(hkaChunkCache *self, unsigned int key, unsigned int size);             /* +12 */
    unsigned char *(*allocateChunk)(hkaChunkCache *self, unsigned int key, unsigned int size);                   /* +16 */
    hkBool *(*flushKey)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);               /* +20 */
    hkBool *(*lockKeyForRead)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);         /* +24 */
    hkBool *(*unlockKeyForRead)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);       /* +28 */
    hkBool *(*lockKeyForWrite)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);        /* +32 */
    hkBool *(*unlockKeyForWrite)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);      /* +36 */
    hkBool *(*isKeyLockedForWrite)(hkBool *result, hkaChunkCache *self, unsigned int key, unsigned int size);    /* +40 */
    void (*enterCriticalSection)(hkaChunkCache *self);                                                           /* +44 */
    void (*leaveCriticalSection)(hkaChunkCache *self);                                                           /* +48 */
    hkBool *(*printCacheStats)(hkBool *result, hkaChunkCache *self, struct hkOstream *);                         /* +52 */
};

/* size 8 — just the hkReferencedObject base. */
struct hkaChunkCache : hkReferencedObject
{
};
