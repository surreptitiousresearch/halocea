#include "../headers/havok/hkStringMap.h"

/* Deeper callee kept as a boundary extern (2-level descent limit): the cached
   hash-map lookup that the string-map thunk forwards to. The C-string pointer is
   passed straight through as the integer hash key. */
extern unsigned int hkCachedHashMap_getWithDefault(const hkCachedHashMap<hkStringMapOperations> *map, unsigned int key, unsigned int defaultValue);
    /* hkCachedHashMap<hkStringMapOperations>::getWithDefault */

/* hkStringMap<T>::getWithDefault — thin thunk to the underlying cached hash map;
   returns the mapped value for `key`, or `defaultValue` when absent. */
void *hkStringMap_getWithDefault(const hkStringMapBase *map, unsigned int key, void *defaultValue)
{
    /* callee returns the mapped value as unsigned int; the thunk hands it back as void*. */
    return (void *)hkCachedHashMap_getWithDefault(map, key, (unsigned int)defaultValue);
}

/* FUNCTION_INDEX entry: hkStringMap_getWithDefault @0x83841EB8 (?getWithDefault@?$hkStringMap@PAUGraphicsNode@hkdAssetProcessingUtil@@@@QBAPAUGraphicsNode@hkdAssetProcessingUtil@@PBDPAU23@@Z)

   Deliberately at the FOOT of the file, not the head: this TU carries
   line-keyed entries in .sweep/gapgate_excluded/cpp_castaudit.txt (L14) and
   .sweep/gapgate_excluded/ptrtrunc.txt (:14:) for the cast on the return line,
   and inserting anything above that line silently drifts both keys — which
   un-excludes a HARD cpp_castaudit finding. Keep new prose down here.

   This TU is the flattened generic (its parameter type is hkStringMapBase). The
   image carries 18 per-instantiation hkStringMap<T>::getWithDefault symbols and
   every one of them is the SAME single instruction — `b
   hkCachedHashMap<hkStringMapOperations>::getWithDefault` — so the address above
   is not a choice between differing bodies. The instantiation named is the one
   this corpus actually reaches: both call sites in
   src/havok/buildBreakableShapeRecursively.cpp pass &graphicsNodes->m_map and
   take the result as hkdAssetProcessingUtil_GraphicsNode*, and
   hkStringMap<hkdAssetProcessingUtil::GraphicsNode*> is the unique instantiation
   over that T. */
