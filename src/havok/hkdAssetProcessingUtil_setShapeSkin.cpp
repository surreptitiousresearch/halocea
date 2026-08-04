#include "../headers/havok/hkpShapeSkinningUtil_boundary.h"
#include "../headers/havok/hkMeshShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkThreadMemory.h"

/* Skin a mesh to the shapes of a broken assembly: gather each instance's physics
   shape and world transform into two stack-allocated parallel arrays, fill an
   hkpShapeSkinningUtil::Input (4 bones/vertex, [maxInside 0, maxOutside
   smoothingDistance]) and weight the mesh vertices to the nearest shapes.

   DEVIATION: float-slot-skip -- the decompiler modelled smoothingDistance as a
   double and fabricated a duplicate array parameter; the real signature is
   (dispatcher, mesh, smoothingDistance, transformedShapes). Both arrays are
   stack-allocated from the thread memory bump pointer (raw offsets preserved:
   +8 current, +10 frame base, +11 end; vtable +20 = expand, +24 = pop frame). */
void hkdAssetProcessingUtil_setShapeSkin(hkpCollisionDispatcher *collisionDispatcher, hkMeshShape *skinnedMesh,
                                         float smoothingDistance, const hkArray_hkdShapeInstanceInfo *transformedShapes)
{
    int *tm = (int *)hkThreadMemory_getCurrent();
    int numBones = transformedShapes->m_size;
    hkpShapeSkinningUtil_Input input;
    const hkpShape **shapes;
    hkTransform *transforms;
    unsigned int shapesBytes = (4 * (numBones + 4)) & 0xFFFFFFF0;
    char *transformsEnd;
    int i;

    input.m_maxOutside = smoothingDistance;
    input.m_bonesPerVertex = 4;
    input.m_maxInside = 0.0f;
    input.m_collisionDispatcher = 0; /* memset(&m_collisionDispatcher, 0, 16) */
    input.m_shapes = 0;
    input.m_transforms = 0;
    input.m_numBones = 0;
    input.m_vertexPositions = 0;
    input.m_numVertices = 0;

    /* stack-allocate the shape-pointer array */
    shapes = (const hkpShape **)tm[8];
    if ((unsigned int)shapes + shapesBytes > (unsigned int)tm[11])
        shapes = (const hkpShape **)(*(int (__fastcall **)(int *))((*(int *)tm) + 20))(tm);
    else
        tm[8] = (int)((char *)shapes + shapesBytes);

    /* stack-allocate the transform array */
    tm = (int *)hkThreadMemory_getCurrent();
    transforms = (hkTransform *)tm[8];
    transformsEnd = (char *)transforms + (((numBones << 6) + 16) & 0xFFFFFFF0);
    if ((unsigned int)transformsEnd > (unsigned int)tm[11])
        transforms = (hkTransform *)(*(int (__fastcall **)(int *))((*(int *)tm) + 20))(tm);
    else
        tm[8] = (int)transformsEnd;

    input.m_transforms = transforms;
    input.m_shapes = shapes;
    input.m_numBones = numBones;
    input.m_collisionDispatcher = collisionDispatcher;

    for (i = 0; i < numBones; ++i)
    {
        const hkdShapeInstanceInfo *inst = &transformedShapes->m_data[i];
        shapes[i] = inst->m_shape->m_physicsShape.m_pntr;
        transforms[i] = inst->m_transform;
    }

    hkpShapeSkinningUtil_setSkinningValues(&input, skinnedMesh);

    /* restore both stack allocators (transforms allocated last, freed first) */
    tm = (int *)hkThreadMemory_getCurrent();
    tm[8] = (int)transforms;
    if (transforms == (hkTransform *)tm[10])
        (*(void (__fastcall **)(int *, hkTransform *))((*(int *)tm) + 24))(tm, transforms);
    tm = (int *)hkThreadMemory_getCurrent();
    tm[8] = (int)shapes;
    if (shapes == (const hkpShape **)tm[10])
        (*(void (__fastcall **)(int *, const hkpShape **))((*(int *)tm) + 24))(tm, shapes);
}
