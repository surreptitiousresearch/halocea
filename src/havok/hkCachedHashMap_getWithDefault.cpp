// FUNCTION_INDEX entry: hkCachedHashMap_getWithDefault @0x838E8980 (?getWithDefault@?$hkCachedHashMap@UhkStringMapOperations@@@@QBAKKK@Z)
#include "../headers/havok/hkStringMap.h"

/* hkCachedHashMap<hkStringMapOperations>::findKey — probe the table for key and
   return its slot index (a value greater than m_hashMod means "not found").
   Boundary — sibling method in the same instantiation. */
extern int hkCachedHashMap_findKey(const hkCachedHashMap<hkStringMapOperations> *self, unsigned int key);

/* hkCachedHashMap<hkStringMapOperations>::getWithDefault — look up key and return
   its stored value, or def when the key is absent. The open-addressed table lays
   the values immediately after the (2*m_hashMod + 2)-entry key region, so the
   value for slot i is m_elem[2*m_hashMod + 2 + i]. */
unsigned int hkCachedHashMap_getWithDefault(const hkCachedHashMap<hkStringMapOperations> *self,
                                            unsigned int key, unsigned int def)
{
    int idx = hkCachedHashMap_findKey(self, key);
    int hashMod = self->m_hashMod;
    if (idx > hashMod)
        return def;
    return self->m_elem[2 * hashMod + 2 + idx];
}
