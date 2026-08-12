#include "../headers/havok/hkMeshShape.h"

/* hkMeshShape is an abstract interface; getNumSections is pure virtual
   (vtable slot 0x0C), implemented by concrete shapes such as
   hkMemoryMeshShape / hkDefaultCompoundMeshShape. This wrapper is the virtual
   dispatch the frontier callers (matchGraphicsToGeometry,
   hkdMeshSplittingPlaneUtil_buildMeshSections) reach through the extern. */
typedef struct hkMeshShape_vtbl
{
    void (*dtor)(hkMeshShape *self);                    /* 0x00 */
    const void *(*getClassType)(hkMeshShape *self);     /* 0x04 */
    void (*calcContentStatistics)(hkMeshShape *self, void *, const void *); /* 0x08 */
    int (*getNumSections)(hkMeshShape *self);           /* 0x0C */
} hkMeshShape_vtbl;

int hkMeshShape_getNumSections(const hkMeshShape *self)
{
    hkMeshShape *s = (hkMeshShape *)self;
    return ((hkMeshShape_vtbl *)s->__vftable)->getNumSections(s);
}

/* FUNCTION_INDEX entry: hkMeshShape_getNumSections @0x83873584 (inlined in hkdMeshSplittingPlaneUtil_buildMeshSections)

   Deliberately at the FOOT of the file: this TU carries line-keyed entries in
   .sweep/gapgate_excluded/cpp_castaudit.txt (L18, L19) for the two casts above,
   and inserting anything ahead of them drifts both keys, which un-excludes a
   HARD cpp_castaudit finding. Keep new prose down here.

   No out-of-line `?getNumSections@hkMeshShape@@` exists — `funcs`/`names` carry
   only the five concrete overrides (hkMemoryMeshShape 0x839F1D50,
   hkDefaultCompoundMeshShape 0x839F24D0, hkMeshSectionBuilder 0x83874EA0,
   hkBinaryPackfileReader 0x839A2450, hkMeshSectionLockSet 0x839E4CE8) — so this
   wrapper materialises the DISPATCH and every address it can state is interior
   to a caller. The one cited is the slot-0x0C dispatch inside
   hkdMeshSplittingPlaneUtil_buildMeshSections (entry at 0x83873528),
   immediately after hkMeshSectionLockSet::addMeshSections, matching this TU's
   caller line for line:
       at 0x83873584  lwz   r11, 0(r30)      ; vtable
       at 0x83873588  mr    r3, r30          ; this
       at 0x8387358C  lwz   r10, 0xC(r11)    ; slot 0x0C = getNumSections
       at 0x83873590  mtctr r10
       at 0x83873594  bctrl
   Naming one of the concrete overrides instead would index the wrapper as one
   particular subclass, which is the guess this convention exists to refuse. */
