#include "../headers/havok/hkpListShape.h"
#include "../headers/havok/hkThreadMemory.h"

/* HK_MEMORY_CLASS_CDINFO — collision-detection shape allocator class. DB exposes
   the enumerator NAME but not its value; the inlined call site loads it as the
   literal 0x28 (== 40), confirmed identical to the CDINFO alloc in
   hkpConvexVerticesConnectivityUtil::_findConnectivity @ 0x83F93368. */
extern const int HK_MEMORY_CLASS_CDINFO;

/* hkpListShape::hkpListShape(const hkpShape* const*, int, ReferencePolicy). */
extern void hkpListShape_construct(hkpListShape *self, const hkpShape *const *shapes,
                                   int numShapes, int referencePolicy);

/* hkpListShape_create — allocate-and-construct factory for an hkpListShape.
   Inline-only in the shipped binary; body reconstructed from the sole inlined
   call site in hkpShapeCutterUtil::cut @ 0x83F97B94-0x83F97BC4:

     0x83F97B94  lwz  r11, 0(r13)                 ; TLS thread base
     0x83F97B98  li   r10, 0x2C0
     0x83F97B9C  li   r5, 0x28                     ; HK_MEMORY_CLASS_CDINFO
     0x83F97BA0  li   r4, 0x70                     ; 112 = sizeof(hkpListShape)
     0x83F97BA4  lwzx r3, r10, r11                 ; hkThreadMemory::getCurrent()
     0x83F97BA8  bl   hkThreadMemory::allocateChunk(112, HK_MEMORY_CLASS_CDINFO)
     0x83F97BAC  li   r9, 0x70
     0x83F97BB4  sth  r9, 4(r3)                    ; m_memSizeAndFlags = 112
     0x83F97BB0  li   r6, 1                         ; ReferencePolicy = REFERENCE_POLICY_INCREMENT
     0x83F97BB8  lwz  r5, numShapes
     0x83F97BBC  lwz  r4, shapeArray
     0x83F97BC0  bl   hkpListShape::hkpListShape(shapeArray, numShapes, 1)  ; ctor @ 0x83F97BC0

   DEVIATION: reconstructed from the inlined site; the raw store `sth r9,4(r3)` is
   expressed through the typed hkReferencedObject::m_memSizeAndFlags field
   (offset 4, reached via base.base), and the TLS getCurrent deref through the
   hkThreadMemory_getCurrent accessor. The size/flags word is written by the
   allocator wrapper BEFORE the constructor runs, matching the call-site order. */
hkpListShape *hkpListShape_create(const hkpShape *const *shapes, int count)
{
    hkpListShape *self = (hkpListShape *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), sizeof(hkpListShape), HK_MEMORY_CLASS_CDINFO);

    self->base.base.m_memSizeAndFlags = 0x70; /* 112 = allocation size, per sth r9,4(r3) */

    hkpListShape_construct(self, shapes, count, REFERENCE_POLICY_INCREMENT);
    return self;
}
