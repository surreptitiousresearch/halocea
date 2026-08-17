/* SphereGun_extractSphereShape @0x8385E8B8 */
/* =========================================================================
   NOTE / CAVEAT: the algorithm flow below is traced faithfully, but several
   boundary objects (hkdSphereExtractorVolume, hkdShapeExtractorUtil::Info,
   hkpShapeConnectedCalculator) are modelled opaquely with stack-derived sizes,
   and the sphere volume's centre/radius field offsets are best-effort. Also a
   float-slot-skip fix: the decompiler modelled `radius` as a double and split
   the output into a typed phantom `shapesOut` plus a raw `a6` -- the real 5th
   argument is the shapesOut array the compounds are written to. The seven
   working arrays are hkInplaceArray<T,16> (inline storage, cap 0x80000010 =
   dont-deallocate | 16); they only heap-allocate if they outgrow 16.
   ========================================================================= */
#include "../headers/havok/hkShapeExtractor_boundary.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkpWorld.h"

typedef struct hkpShape hkpShape;

extern const hkTransform hkTransformIdentity;
extern void SphereGun_subtractCuboid(const hkAabb *aabb, const hkpShape *shape,
                                     const hkpShape **insideShapeOut, hkArray<> *outsideShapes);
extern void hkArrayUtil__reserveMore(void *array, int elementSize);

static void inplace_init(hkArray<> *a, void *storage)
{
    a->m_data = storage;
    a->m_size = 0;
    a->m_capacityAndFlags = 0x80000010; /* inline storage, capacity 16 */
}

static void inplace_free(hkArray<> *a, int elementSize)
{
    if ((int)a->m_capacityAndFlags >= 0) /* only if it grew onto the heap */
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), a->m_data,
                                       elementSize * a->m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
}

/* Extract the part of `shape` inside the sphere (pos, radius) as a set of
   connected compound shapes appended to shapesOut. Returns 1 if any compound was
   produced (the shape overlapped the sphere), 0 otherwise. */
int SphereGun_extractSphereShape(hkpWorld *world, const hkpShape *shape, const hkVector4 *pos, float radius,
                                 hkArray<> *shapesOut /* hkArray<const hkpShape*> */)
{
    hkAabb sphereBox;
    hkArray<> outside;        unsigned char outsideStore[64];
    const hkpShape *insidePiece = 0;
    int i;

    for (i = 0; i < 4; ++i)
    {
        sphereBox.m_min.m_quad.___u0.v[i] = pos->m_quad.___u0.v[i] - radius;
        sphereBox.m_max.m_quad.___u0.v[i] = pos->m_quad.___u0.v[i] + radius;
    }

    inplace_init(&outside, outsideStore);
    SphereGun_subtractCuboid(&sphereBox, shape, &insidePiece, &outside);

    if (!insidePiece)
    {
        hkReferencedObject_removeReferences(outside.m_data, outside.m_size, 4);
        inplace_free(&outside, 4);
        return 0;
    }

    /* recover the shapes that actually fall inside the sphere volume */
    {
        hkdSphereExtractorVolume volume;
        hkdShapeExtractorUtil_Info info;

        volume.__vftable = (hkBaseObject_vtbl *)&hkdSphereExtractorVolume_vftable;
        volume.m_sphere = *pos;                        /* centre (radius carried in w by caller) */
        /* radius is carried alongside; stored opaquely by the original */
        (void)radius;

        hkdShapeExtractorUtil_Info_construct(&info, &volume, 4, 1);
        hkdShapeExtractorUtil_extractRecurse(&sphereBox, insidePiece, &info, 0);
        hkReferencedObject_addReferences(info.m_shapes.m_data, info.m_shapes.m_size, 4);
        hkArray_spliceInto(&outside, 0, 0, info.m_shapes.m_data, info.m_shapes.m_size);
        hkReferencedObject_removeReference((hkReferencedObject *)insidePiece);
        hkdShapeExtractorUtil_Info_destruct(&info);
    }

    {
        hkArray<> convexShapes;
        unsigned char convexStore[64];
        hkArray<> transforms;
        unsigned char transformStore[16 * 64];
        hkArray<> islandSizes;
        unsigned char islandSizesStore[64];
        hkArray<> islandMembers;
        unsigned char islandMembersStore[64];
        hkArray<> islandConvex;
        unsigned char islandConvexStore[64];
        hkArray<> islandTransforms;
        unsigned char islandTransformStore[16 * 64];
        hkpShapeConnectedCalculator calc;
        hkpCollisionDispatcher *dispatcher = world->m_collisionDispatcher;
        int memberBase = 0;
        int island;

        inplace_init(&convexShapes, convexStore);
        inplace_init(&transforms, transformStore);

        /* decompose every outside/extracted shape into convex pieces + transforms */
        for (i = 0; i < outside.m_size; ++i)
            hkpShapeCutterUtil_flattenIntoConvexShapes(((const hkpShape **)outside.m_data)[i],
                                                       &hkTransformIdentity, &transforms, &convexShapes);

        hkpShapeConnectedCalculator_construct(&calc, dispatcher, 0.25f);

        inplace_init(&islandSizes, islandSizesStore);
        inplace_init(&islandMembers, islandMembersStore);
        hkpShapeCutterUtil_findConnectedIslands(&calc, &convexShapes, &transforms, &islandSizes, &islandMembers);

        inplace_init(&islandConvex, islandConvexStore);
        inplace_init(&islandTransforms, islandTransformStore);

        /* one compound shape per connected island */
        for (island = 0; island < islandSizes.m_size; ++island)
        {
            int count = ((int *)islandSizes.m_data)[island];
            const hkpShape *compound;
            int m;

            islandConvex.m_size = 0;
            islandTransforms.m_size = 0;
            for (m = 0; m < count; ++m)
            {
                int idx = ((int *)islandMembers.m_data)[memberBase + m];
                if (islandConvex.m_size == (islandConvex.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&islandConvex, 4);
                ((const hkpConvexShape **)islandConvex.m_data)[islandConvex.m_size++] =
                    ((const hkpConvexShape **)convexShapes.m_data)[idx];
                if (islandTransforms.m_size == (islandTransforms.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&islandTransforms, 64);
                ((hkTransform *)islandTransforms.m_data)[islandTransforms.m_size++] =
                    ((hkTransform *)transforms.m_data)[idx];
            }

            compound = hkpShapeCutterUtil_createCompound((const hkpConvexShape **)islandConvex.m_data,
                                                         (const hkTransform *)islandTransforms.m_data, islandConvex.m_size);
            if (shapesOut->m_size == (shapesOut->m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(shapesOut, 4);
            ((const hkpShape **)shapesOut->m_data)[shapesOut->m_size++] = compound;
            memberBase += count;
        }

        inplace_free(&islandTransforms, 64);
        inplace_free(&islandConvex, 4);
        inplace_free(&islandMembers, 4);
        inplace_free(&islandSizes, 4);
        inplace_free(&transforms, 64);
        inplace_free(&convexShapes, 4);
    }

    hkReferencedObject_removeReferences(outside.m_data, outside.m_size, 4);
    inplace_free(&outside, 4);
    return 1;
}
