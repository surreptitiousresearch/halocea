#include "../headers/havok/hkpShapeContainer.h"

/* hkpShapeContainer::getFirstKey — pure-virtual child-iteration accessor. Returns
   the shape key of the container's first child, or -1 when the container is empty.

   Out-of-line reconstruction of the inline-only helper. The dispatch was inlined
   into hkpConvexVerticesConnectivityUtil::ensureConnectivityAll @0x83F93930:
       0x83F9395C addi  r31, r31, 0x10   ; container = shape + 0x10
       0x83F93960 mr    r3, r31          ; this = container
       0x83F93964 lwz   r11, 0(r31)      ; r11 = container->__vftable
       0x83F93968 lwz   r10, 8(r11)      ; r10 = vtbl[+0x08] = getFirstKey
       0x83F9396C mtctr r10
       0x83F93970 bctrl                  ; r3 = (int)getFirstKey(this)
   DEVIATION: the decompiler read the vtable load as a raw slot off the shape body;
   modelled here as a typed vtable dispatch through hkpShapeContainer_vtbl (slot at
   offset 0x08). The struct-pointer cast of the opaque __vftable word is not a
   function-pointer cast. */

extern int hkpShapeContainer_getFirstKey(const hkpShapeContainer *container);

int hkpShapeContainer_getFirstKey(const hkpShapeContainer *container)
{
    const hkpShapeContainer_vtbl *vtbl =
        (const hkpShapeContainer_vtbl *)container->__vftable;
    return vtbl->getFirstKey(container);
}
