#include "../headers/havok/hkdGraphicsShape.h"

/* hkMeshShape is opaque here; only a pointer to it is stored/returned. */
typedef struct hkMeshShape hkMeshShape;

/* hkdMeshGraphicsShape::getMesh — inline-only field accessor.
   Reconstructed from the inlined call site in
   hkdAssetProcessingUtil::createBreakableShape @ 0x8383A46C:
       lwz  r3, 0x10(r28)    ; r28 = self (hkdGraphicsShape*), 0x10 = mesh field
   Store side confirmed in the hkdMeshGraphicsShape ctor @ 0x83872BE8:
       stw  r30, 0x10(r31)   ; r30 = ctor arg hkMeshShape*, r31 = this
   The mesh pointer sits immediately after the hkdGraphicsShape base
   (sizeof(hkdGraphicsShape) == 0x10, per DB type hkdGraphicsShape). */
hkMeshShape *hkdMeshGraphicsShape_getMesh(const hkdGraphicsShape *self)
{
    return *(hkMeshShape * const *)((const char *)self + sizeof(hkdGraphicsShape));
}
