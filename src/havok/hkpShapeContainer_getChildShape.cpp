#include "../headers/havok/hkpShapeContainer.h"
#include "../headers/havok/hkpShape.h"

/* hkpShapeContainer::getChildShape — pure-virtual accessor that fetches the
   child hkpShape* for a given shape key, writing any transient sub-shape into the
   caller-provided buffer. Out-of-line dispatch shim for the inline-only call site
   in hkpConvexVerticesConnectivityUtil::ensureConnectivityAll.

   Ground truth (disasm 0x83F93930, the getChildShape block):
       0x83F93980  lwz  r11, 0(r31)     ; vtbl = *(void**)container
       0x83F93984  addi r5, r1, ...     ; r5 = buffer
       0x83F93988  mr   r4, r30         ; r4 = key
       0x83F9398C  mr   r3, r31         ; r3 = container (this)
       0x83F93990  lwz  r10, 0x14(r11)  ; slot 0x14 = getChildShape
       0x83F93994  mtctr r10
       0x83F93998  bctrl               ; -> r3 = const hkpShape*
   DEVIATION: the caller's disasm reads this as a raw vtable slot off a shape
   sub-object pointer; modelled here as the named virtual through the container
   vtable (see hkpShapeContainer.h). */
const hkpShape *hkpShapeContainer_getChildShape(const hkpShapeContainer *container,
                                                int key, void *buffer)
{
    const hkpShapeContainer_vtbl *vtbl = (const hkpShapeContainer_vtbl *)container->__vftable;
    return vtbl->getChildShape(container, key, buffer);
}
