/* CAVEAT: the two backing arrays are released through a per-frame heap interface obtained from
 * the engine-root pointer held in the small-data anchor slot (disasm: `lwz r31,0(r13)` then
 * `lwzx r3,0x2C0,r31` -> heap at root+0x2C0), whose vtable slot +1 is Free(self, ptr). The root
 * pointer and the heap layout are modeled as the `apSysRoot` / `dlHEAP_IFACE` boundary in
 * anim_boundary.h; the raw indirections are reproduced faithfully but the concrete allocator
 * type is not reversed. */
#include "animINST.h"
#include "animTPL.h"
#include "anim_boundary.h"
#include "../m3d/m3dMATR.h"

// ?FreeCollidables@animINST@@QAAXXZ  (0x8291A5F8) — tear down the Havok collidables and the
// local-transform matrix array this instance allocated for its collision mopps.
void animINST::FreeCollidables()
{
    hkpCollidable **pCollidables = (hkpCollidable **)this->pCollidableData;
    if (!pCollidables)
        return;

    int nElem = this->pTpl->coll.mopps.nElem;
    if (nElem > 0)
    {
        for (int i = 0; i < nElem; ++i)
        {
            hkpCollidable *collidable = pCollidables[i];
            if (collidable)
            {
                hkpCollidable_scalar_deleting_dtor(collidable, 1u);
                RemoveCollidablesAllocated();
            }
            pCollidables[i] = nullptr;
        }
    }

    dlHEAP_IFACE *heap = *(dlHEAP_IFACE **)(apSysRoot + 0x2C0);
    sizeMatrLT_COPY -= nElem << 6; // nElem * sizeof(m3dMATR)
    heap->__vftable->Free(heap, this->pMatrCollidables);
    this->pMatrCollidables = nullptr;

    heap = *(dlHEAP_IFACE **)(apSysRoot + 0x2C0);
    heap->__vftable->Free(heap, this->pCollidableData);
    this->pCollidableData = nullptr;
}
