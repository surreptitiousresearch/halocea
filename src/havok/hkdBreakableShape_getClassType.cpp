#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkClass.h"

/* hkdBreakableShape::getClassType — inline-only in the binary; reconstructed
   from the sole inlined call site in createDeformableShape @0x8383F840:

       0x8383F878  lwz   r11, 0(r3)   ; r11 = self->__vftable
       0x8383F87C  lwz   r10, 4(r11)  ; r10 = vtable slot at byte offset 4
       0x8383F880  mtctr r10
       0x8383F884  bctrl              ; const hkClass* (*)(const hkdBreakableShape*)

   This is a genuine virtual dispatch: the class has no non-virtual accessor in
   the DB, only per-subtype overrides (e.g. hkdDeformableBreakableShape's at
   0x838645E8). The shim reproduces the vtable read + call exactly.

   DEVIATION: the vtable slot is modeled as a typed function-pointer member at
   byte offset 4 (slot index 1) so the dispatch is expressed without a
   function-pointer cast; only a data-pointer cast of __vftable is used. */

struct hkdBreakableShape_ClassTypeVtbl
{
    void *reserved0;                                            /* +0 */
    const hkClass *(*getClassType)(const hkdBreakableShape *);  /* +4 (per disasm lwz r10,4(r11)) */
};

const hkClass *hkdBreakableShape_getClassType(const hkdBreakableShape *self)
{
    const hkdBreakableShape_ClassTypeVtbl *vtbl =
        (const hkdBreakableShape_ClassTypeVtbl *)self->__vftable;
    return vtbl->getClassType(self);
}
