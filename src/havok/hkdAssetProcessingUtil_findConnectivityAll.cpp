#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape_Connection.h"
#include "../headers/havok/hkInplaceArray.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hk1AxisSweep.h"
#include "../headers/havok/hkdConnectivityCalculator.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkDefaultError.h"
#include "../headers/havok/hkpShape.h"

/* Boundary callees (2-level descent limit). */
extern void hkError_setEnabled(hkDefaultError *self, int id, void *enabled);                /* hkError::setEnabled */
extern void hkdBreakableShape_getChildShapesRecursively(const hkdBreakableShape *self, const hkTransform *parent,
                                                        hkArray<hkdShapeInstanceInfo> *out);
extern void hkpShape_getAabb(const hkpShape *self, const hkTransform *localToWorld, float tolerance, hkAabb *out); /* virtual getAabbImpl */
extern int  hkdBreakableShape_isDescendantOf(const hkdBreakableShape *self, const hkdBreakableShape *other);
extern const hkdBreakableShape *hkdBreakableShape_findCommonParent(const hkdBreakableShape *a, const hkdBreakableShape *b);
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem);
extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *v);
extern void hkVector4_setTransformedInversePos(hkVector4 *out, const hkTransform *t, const hkVector4 *v);
extern void hkdBreakableShape_addConnection(hkdBreakableShape *self, const hkdBreakableShape_Connection *connection);
extern const hkTransform hkTransformIdentity;

#define CONNECTIVITY_ERROR_ID (-1357717232)

/* hkdAssetProcessingUtil::findConnectivityAll — compute and record the physical
   connections between all leaf shapes of a breakable shape tree. Leaf AABBs are
   swept (sweep-and-prune) to find candidate overlapping pairs; each pair that is
   not an ancestor/descendant relationship and passes its common parent's
   connectivity-type rules is passed to the connectivity calculator, and a
   sufficiently strong contact becomes a Connection on that common parent. */
void hkdAssetProcessingUtil_findConnectivityAll(hkpCollisionDispatcher *dispatcher,
        const hkdBreakableShape *shape, float distanceForConnection, float minArea)
{
    hkInplaceArray<hkdShapeInstanceInfo, 8> leafShapes;
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    hk1AxisSweep_AabbInt *aabbInts;
    hkArray<hkKeyPair> pairs;
    int numLeaf, i, numPairs, remaining;

    hkError_setEnabled((hkDefaultError *)hkError_s_instance, CONNECTIVITY_ERROR_ID, 0); /* silence connectivity spew */

    leafShapes.m_data = leafShapes.m_storage;
    leafShapes.m_size = 0;
    leafShapes.m_capacityAndFlags = (int)0x80000008;
    hkdBreakableShape_getChildShapesRecursively(shape, &hkTransformIdentity, &leafShapes);
    numLeaf = leafShapes.m_size;

    /* Scratch AABB list (+4 sentinel slots), from the per-thread stack allocator. */
    aabbInts = (hk1AxisSweep_AabbInt *)hkThreadMemory_allocateStack(tm, (numLeaf + 4) * (int)sizeof(hk1AxisSweep_AabbInt));
    for (i = 0; i < numLeaf; ++i)
    {
        hkAabb aabb;
        const hkpShape *physics = leafShapes.m_data[i].m_shape->m_physicsShape.m_pntr;
        hkpShape_getAabb(physics, &leafShapes.m_data[i].m_transform, distanceForConnection * 0.5f, &aabb);
        hk1AxisSweep_AabbInt_set(&aabbInts[i], &aabb, i);
    }
    for (i = 0; i < 4; ++i)
        aabbInts[numLeaf + i].m_min[0] = 0xFFFFFFFF; /* end-of-list sentinels */
    hk1AxisSweep_sortAabbs(aabbInts, numLeaf);

    /* Collect overlapping pairs, growing the output until the sweep completes. */
    pairs.m_data = 0;
    pairs.m_size = 0;
    pairs.m_capacityAndFlags = (int)0x80000000;
    numPairs = 0;
    remaining = 16 * numLeaf;
    while (remaining)
    {
        int cap = pairs.m_capacityAndFlags & 0x3FFFFFFF;
        int need = cap + remaining;
        if (cap < need)
        {
            int grow = 2 * cap;
            if (need >= grow) grow = need;
            hkArrayUtil__reserve(&pairs.m_data, grow, (int)sizeof(hkKeyPair));
        }
        numPairs = hk1AxisSweep_collide(aabbInts, numLeaf, pairs.m_data,
                                        pairs.m_capacityAndFlags & 0x3FFFFFFF, &remaining);
        cap = pairs.m_capacityAndFlags & 0x3FFFFFFF;
        if (cap < numPairs)
        {
            int grow = 2 * cap;
            if (numPairs >= grow) grow = numPairs;
            hkArrayUtil__reserve(&pairs.m_data, grow, (int)sizeof(hkKeyPair));
        }
    }

    /* Return the AABB scratch to the stack allocator before the heavy per-pair work. */
    hkThreadMemory_deallocateStack(tm, aabbInts);

    for (i = 0; i < numPairs; ++i)
    {
        hkdShapeInstanceInfo *leafA = &leafShapes.m_data[pairs.m_data[i].m_keyA];
        hkdShapeInstanceInfo *leafB = &leafShapes.m_data[pairs.m_data[i].m_keyB];
        const hkdBreakableShape *commonParent;
        const hkdBreakableShape *node;
        int useConnectivity = 0;

        if (hkdBreakableShape_isDescendantOf(leafA->m_shape, leafB->m_shape))
            continue;
        if (hkdBreakableShape_isDescendantOf(leafB->m_shape, leafA->m_shape))
            continue;
        commonParent = hkdBreakableShape_findCommonParent(leafA->m_shape, leafB->m_shape);
        if (!commonParent)
            continue;

        for (node = commonParent; node; node = node->m_parent)
        {
            unsigned int ct = node->m_connectivityType.m_storage;
            if (ct == 0 || ct > 3)
                continue; /* CONNECTIVITY_INHERITED / invalid: keep walking up */
            if (ct == 1) /* CONNECTIVITY_PARTIAL: only siblings connect */
            {
                if (leafA->m_shape->m_parent == leafB->m_shape->m_parent)
                {
                    useConnectivity = 1;
                    break;
                }
                continue;
            }
            /* ct == 2 (FULL) connects; ct == 3 (NONE) does not */
            if (ct == 2)
                useConnectivity = 1;
            break;
        }
        if (!useConnectivity)
            continue;

        {
            hkdConnectivityCalculator_ConnectivityInput in;
            hkdConnectivityCalculator_ConnectivityOutput out;
            in.m_maxDistance = distanceForConnection;
            in.m_dispatcher = dispatcher;
            in.m_simplifyComplexShapeBByPlane.m_bool = 1;
            in.m_shapeA = leafA->m_shape->m_physicsShape.m_pntr;
            in.m_transformA = leafA->m_transform;
            in.m_shapeB = leafB->m_shape->m_physicsShape.m_pntr;
            in.m_transformB = leafB->m_transform;
            if (hkdConnectivityCalculator_calculateSemiAccurateConnectivity(&in, &out))
            {
                float area = out.m_area;
                float volA = leafA->m_shape->m_massProps.m_volume;
                float volB = leafB->m_shape->m_massProps.m_volume;
                float minVol = (volA - volB >= 0.0f) ? volB : volA; /* min(volA, volB) */
                if (area >= minArea && (area * area * area * 1000000.0f) >= (minVol * minVol))
                {
                    hkdBreakableShape_Connection conn;
                    hkVector4 worldPivot;
                    conn.m_a = leafA->m_shape;
                    conn.m_b = leafB->m_shape;
                    conn.m_pivotA = out.m_pivotA;
                    hkVector4_setTransformedPos(&worldPivot, &leafA->m_transform, &out.m_pivotA);
                    hkVector4_setTransformedInversePos(&conn.m_pivotB, &leafB->m_transform, &worldPivot);
                    conn.m_separatingNormal = out.m_separatingNormalA;
                    conn.m_contactArea = area;
                    conn.m_separatingNormal.m_quad.___u0.v[3] = out.m_phi;
                    conn.m_pivotA.m_quad.___u0.v[3] = out.m_sectionModulusY;
                    conn.m_pivotB.m_quad.___u0.v[3] = out.m_sectionModulusZ;
                    hkdBreakableShape_addConnection((hkdBreakableShape *)commonParent, &conn);
                }
            }
        }
    }

    hkError_setEnabled((hkDefaultError *)hkError_s_instance, CONNECTIVITY_ERROR_ID, (void *)1); /* re-enable */

    if ((pairs.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(tm, pairs.m_data,
                                       (int)sizeof(hkKeyPair) * (pairs.m_capacityAndFlags & 0x3FFFFFFF),
                                       HK_MEMORY_CLASS_ARRAY);
    if ((leafShapes.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(tm, leafShapes.m_data,
                                       (int)sizeof(hkdShapeInstanceInfo) * (leafShapes.m_capacityAndFlags & 0x3FFFFFFF),
                                       HK_MEMORY_CLASS_ARRAY);
}
