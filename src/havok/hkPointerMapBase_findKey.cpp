/* ?findKey@?$hkPointerMapBase@KKU?$hkPointerMapOperations@KK@@@@QBAPAVDummy@@K@Z @0x838E4AA0 */
#include "../headers/havok/hkPointerMapBase.h"

/* hkPointerMapBase::findKey — probe for key using the same multiplicative hash and
   linear probing as insert. Returns the occupied slot index holding the key, or
   (m_hashMod + 1) — one past the last valid slot — when the key is absent (a
   probe reached an empty slot). */
unsigned int hkPointerMapBase_findKey(hkPointerMapBase *self, unsigned int key)
{
    int mask = self->m_hashMod;
    hkPointerMapPair *elem = self->m_elem;
    unsigned int slot = (-1640531535u * (key >> 4)) & mask;
    unsigned int probed = elem[slot].key;

    if (probed == (unsigned int)-1)
        return mask + 1;
    while (probed != key)
    {
        slot = (slot + 1) & mask;
        probed = elem[slot].key;
        if (probed == (unsigned int)-1)
            return mask + 1;
    }
    return slot;
}
