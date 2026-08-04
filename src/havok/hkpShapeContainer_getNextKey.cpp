#include "../headers/havok/hkpShapeContainer.h"

/* hkpShapeContainer::getNextKey — advance a shape-collection child iterator.
   Pure-virtual accessor, inlined at its only call site in
   hkpConvexVerticesConnectivityUtil::ensureConnectivityAll (@0x83F93930):

       0x83F939A8  lwz   r11, 0(r31)      ; r11 = container->__vftable
       0x83F939AC  mr    r4, r30          ; r4  = key
       0x83F939B0  mr    r3, r31          ; r3  = container (this)
       0x83F939B4  lwz   r10, 0xC(r11)    ; r10 = vtbl slot 0x0C = getNextKey
       0x83F939B8  mtctr r10
       0x83F939BC  bctrl                  ; return (int) slot(this, key)

   Returns the next child key, or -1 when the iteration is exhausted.

   DEVIATION: reconstructed from the inlined vtable dispatch. The struct models
   the vtable pointer as void* (hkpShapeContainer::__vftable), so the typed slot
   table is reached via a data-pointer cast — no function-pointer cast. */
int hkpShapeContainer_getNextKey(const hkpShapeContainer *container, int key)
{
    const hkpShapeContainer_vtbl *vtable =
        (const hkpShapeContainer_vtbl *)container->__vftable;
    return vtable->getNextKey(container, key);
}
