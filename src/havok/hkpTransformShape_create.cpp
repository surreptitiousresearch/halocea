#include "../headers/havok/hkpTransformShape.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

/* Boundary types (used only through pointers here). */
struct hkpShape;
struct hkpConvexVerticesShape;
struct hkTransform;

/* Memory-class tag used by the shape-cutter allocations (enumerator name only in
   the DB; numeric class at the call site is 0x28). Declared here rather than
   pulling in hkpShapeCutterUtil_boundary.h. */
extern const int HK_MEMORY_CLASS_CDINFO;

/* hkpTransformShape::hkpTransformShape(hkpShape const *, hkTransform const &)
   @0x83613FB0 — authored out-of-line under the flattened extern "C" name in
   src/havok/hkpTransformShape_ctor.cpp. */
extern "C" void hkpTransformShape_ctor(hkpTransformShape *self, const hkpShape *childShape,
                                       const hkTransform *transform);

/* hkpTransformShape_create — allocate + construct a general transform-wrapper
   shape around `child` under transform `t`, returning it with one reference.
   This factory exists ONLY inlined; reconstructed from hkpShapeCutterUtil::cut
   @0x83F97520 case 0xF (the TRANSFORM case), 0x83F97768..0x83F977A0:
     allocateChunk(112, HK_MEMORY_CLASS_CDINFO)  @0x83F97774/0x83F9777C
     sth 0x70 -> m_memSizeAndFlags (offset 4)    @0x83F97780/0x83F97788
     hkpTransformShape::hkpTransformShape(child, t) @0x83F97790 (ctor 0x83613FB0)
   DEVIATION: the caller's post-construct removeReference on the child belongs to
   the surrounding wrap logic in cut(), not to this factory; it is not reproduced
   here. The ctor sets the vtable/refcount; only m_memSizeAndFlags is stamped by
   the factory. */
hkpTransformShape *hkpTransformShape_create(const hkpConvexVerticesShape *child, const hkTransform *t)
{
    hkReferencedObject *chunk = (hkReferencedObject *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 112, HK_MEMORY_CLASS_CDINFO);
    chunk->m_memSizeAndFlags = 112;
    hkpTransformShape_ctor((hkpTransformShape *)chunk, (const hkpShape *)child, t);
    return (hkpTransformShape *)chunk;
}
