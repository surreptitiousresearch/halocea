/* ??0?$hkPointerMapBase@KKU?$hkPointerMapOperations@KK@@@@QAA@PAXH@Z @0x838E4968 */
#include "../headers/havok/hkPointerMapBase.h"

/* hkPointerMapBase::hkPointerMapBase(Pair *ptr, unsigned sizeInBytes) — adopt a
   caller-supplied slot buffer. m_numElems is stamped negative (0x80000000,
   DONT_DEALLOCATE) so the destructor never frees caller storage; m_hashMod is the
   power-of-two slot mask (slotCount - 1). Every slot's key is marked empty (-1).
   slotCount = sizeInBytes / 8 (each Pair is 8 bytes). */
void hkPointerMapBase_construct(hkPointerMapBase *self, hkPointerMapPair *ptr, unsigned int sizeInBytes)
{
    unsigned int slotCount = sizeInBytes >> 3;
    unsigned int i;

    self->m_elem = ptr;
    self->m_numElems = (int)0x80000000;
    self->m_hashMod = slotCount - 1;
    for (i = 0; i < slotCount; ++i)
        self->m_elem[i].key = (unsigned int)-1;
}
