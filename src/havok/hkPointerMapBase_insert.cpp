/* ?insert@?$hkPointerMapBase@KKU?$hkPointerMapOperations@KK@@@@QAAHKK@Z @0x838E49E0 */
#include "../headers/havok/hkPointerMapBase.h"

/* hkPointerMapBase::insert — open-addressing (linear-probe) insert. Grows the
   table first when it is more than half full (2*numElems > hashMod). The hash is
   Knuth's multiplicative constant (0x9E3779B1 == -1640531535) over key>>4, masked
   by m_hashMod. Returns 1 when a new slot was consumed, 0 when an existing key was
   overwritten. */
int hkPointerMapBase_insert(hkPointerMapBase *self, unsigned int key, unsigned int val)
{
    int mask;
    hkPointerMapPair *elem;
    int isNew;
    unsigned int slot;

    if (2 * self->m_numElems > self->m_hashMod)
        hkPointerMapBase_resizeTable(self, 2 * (self->m_hashMod + 1));

    mask = self->m_hashMod;
    elem = self->m_elem;
    isNew = 1;
    slot = (-1640531535u * (key >> 4)) & mask;

    if (elem[slot].key != (unsigned int)-1)
    {
        while (elem[slot].key != key)
        {
            slot = (slot + 1) & mask;
            if (elem[slot].key == (unsigned int)-1)
                goto store;
        }
        isNew = 0; /* key already present — overwrite value below */
    }
store:
    self->m_numElems += isNew;
    elem[slot].key = key;
    elem[slot].val = val;
    return isNew;
}
