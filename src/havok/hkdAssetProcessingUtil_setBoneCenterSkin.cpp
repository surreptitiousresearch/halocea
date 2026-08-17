/* hkdAssetProcessingUtil_setBoneCenterSkin @0x8383C420 */
#include "../headers/havok/hkMeshShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkSkinningUtil_Entry.h"

/* hkMeshVertexBuffer is an opaque boundary type here (the DEF's vertexBuffer arg). */
typedef struct hkMeshVertexBuffer hkMeshVertexBuffer;

extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *v); /* hkVector4::setTransformedPos */
extern void hkSkinningUtil_setSkinningValues(const hkArray<hkSkinningUtil_Entry> *entries, hkMeshVertexBuffer *vertexBuffer, float smoothingDistance); /* hkSkinningUtil::setSkinningValues */

/* Compute one bone-center position per instance (each shape's local center at
   offset 144 transformed into world space) and hand them to the skinning util
   to weight the mesh vertices by nearest bone within smoothingDistance.

   DEVIATION: float-slot-skip — the decompiler modelled smoothingDistance as a
   double and fabricated a duplicate array parameter; the real signature is
   (mesh, smoothingDistance, transformedShapes). The bone-center array is
   stack-allocated from the thread memory's bump pointer (raw offsets preserved:
   +8 current, +10 frame base, +11 end; vtable +20 = expand, +24 = pop frame). */
void hkdAssetProcessingUtil_setBoneCenterSkin(hkMeshShape *skinnedMesh, float smoothingDistance,
                                              const hkArray_hkdShapeInstanceInfo *transformedShapes)
{
    int *tm = (int *)hkThreadMemory_getCurrent();
    int count = transformedShapes->m_size;
    hkArray<hkVector4> boneCenters;
    hkVector4 *scratch;
    int i;

    scratch = (hkVector4 *)tm[8];
    if ((unsigned int)&scratch[count + 1] > (unsigned int)tm[11])
        scratch = (hkVector4 *)(*(int (__fastcall **)(int *))((*(int *)tm) + 20))(tm); /* vtable+20: expand frame */
    else
        tm[8] = (int)&scratch[count + 1];

    boneCenters.m_data = scratch;
    boneCenters.m_size = count;
    boneCenters.m_capacityAndFlags = count | 0x80000000; /* stack-owned: dont-deallocate */

    for (i = 0; i < count; ++i)
    {
        const hkdShapeInstanceInfo *inst = &transformedShapes->m_data[i];
        hkVector4_setTransformedPos(&scratch[i], &inst->m_transform,
                                    (const hkVector4 *)((char *)inst->m_shape + 144));
    }

    /* DEF order is (entries, vertexBuffer, distance): the bone-center array is the
       entries arg and the mesh is the vertex buffer (reconstruction had them swapped). */
    hkSkinningUtil_setSkinningValues((const hkArray<hkSkinningUtil_Entry> *)&boneCenters,
                                     (hkMeshVertexBuffer *)skinnedMesh, smoothingDistance);

    tm[8] = (int)scratch; /* restore bump pointer */
    if (scratch == (hkVector4 *)tm[10])
        (*(void (__fastcall **)(int *))((*(int *)tm) + 24))(tm); /* vtable+24: pop frame */
    if ((int)boneCenters.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), boneCenters.m_data,
                                       16 * boneCenters.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
}
