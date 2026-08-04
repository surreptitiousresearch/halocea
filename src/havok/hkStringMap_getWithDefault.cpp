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
