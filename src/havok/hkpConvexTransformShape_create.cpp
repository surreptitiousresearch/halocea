#include "../headers/havok/hkpConvexTransformShape.h"
#include "../headers/havok/hkpListShape.h"   /* hkpShapeContainerReferencePolicy: REFERENCE_POLICY_INCREMENT */
#include "../headers/havok/hkThreadMemory.h"

/* Opaque child shape type (level-2 boundary — handled only through pointers). */
typedef struct hkpConvexVerticesShape hkpConvexVerticesShape;

/* hkpConvexTransformShape_create — allocate-and-construct factory for an
   hkpConvexTransformShape. This helper is inline-only in the shipped binary: it has NO
   entry of its own in the image, so the address below is an interior citation of another
   function's body. Reconstructed from the sole inlined call site in
   hkpShapeCutterUtil::cut, 0x83F97928-0x83F97958:
   @0x83F97928 (inlined in ?cut@hkpShapeCutterUtil@@SAPBVhkpShape@@PBV2@ABVhkVector4@@M@Z)

     0x83F97928  lwz  r11, 0(r13)                 ; TLS thread base
     0x83F9792C  li   r10, 0x2C0
     0x83F97930  li   r5, 0x28                     ; HK_MEMORY_CLASS_CDINFO
     0x83F97934  li   r4, 0x60                     ; 96 = sizeof(hkpConvexTransformShape)
     0x83F97938  lwzx r3, r10, r11                 ; hkThreadMemory::getCurrent()
     0x83F9793C  bl   hkThreadMemory::allocateChunk(96, HK_MEMORY_CLASS_CDINFO)
     0x83F97940  li   r9, 0x60
     0x83F97948  sth  r9, 4(r3)                    ; m_memSizeAndFlags = 96
     0x83F97944  li   r6, 1                         ; ReferencePolicy = REFERENCE_POLICY_INCREMENT
     0x83F97954  bl   hkpConvexTransformShape::hkpConvexTransformShape(child, transform, 1)
                                                    ; ctor @ 0x83619CA0

   DEVIATION: reconstructed from the inlined site; the raw store `sth r9,4(r3)`
   is expressed through the typed hkReferencedObject::m_memSizeAndFlags field
   (offset 4), and the TLS getCurrent deref through the hkThreadMemory_getCurrent
   accessor. The size/flags word is written by the allocator wrapper BEFORE the
   constructor runs, matching the instruction order at the call site. */
hkpConvexTransformShape *hkpConvexTransformShape_create(const hkpConvexVerticesShape *child,
                                                        const hkTransform *t)
{
    hkpConvexTransformShape *self = (hkpConvexTransformShape *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), sizeof(hkpConvexTransformShape), HK_MEMORY_CLASS_CDINFO);

    self->base.base.m_memSizeAndFlags = 0x60; /* 96 = allocation size, per sth r9,4(r3) */

    hkpConvexTransformShape_construct(self, (const hkpShape *)child, t, REFERENCE_POLICY_INCREMENT);
    return self;
}
