#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape_Connection.h"
#include "../headers/havok/hkdControllerUtil_ConnectionRef.h"
#include "../headers/havok/hkPointerMap.h"
#include "../headers/havok/hkInplaceArray.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* hkPointerMapBase type + operations come from hkPointerMapBase.h (via hkPointerMap.h);
   the local redefinitions were removed to avoid C2011/C2371 clashes with the DB-verified
   header declarations. */

/* A (parent shape, referencing-child count) pair collected while scanning the
   input shapes' ancestor chains. */
typedef struct ParentCount { const hkdBreakableShape *m_parent; int m_count; } ParentCount;

/* hkdControllerUtil::findConnections — for a set of leaf shape instances, emit
   the connection references that join pairs of those leaves. A shape->index map
   is built (into shapesMapOut, or a local scratch map when null); every ancestor
   shared by more than one input leaf is examined, and each of its connections
   whose endpoints are both in the input set produces a ConnectionRef. */
void hkdControllerUtil_findConnections(const hkdShapeInstanceInfo *shapesIn, int numShapesIn,
                                       hkArray<hkdControllerUtil_ConnectionRef> *connectionsOut,
                                       void *shapesMapOut)
{
    hkPointerMap<const hkdBreakableShape *, int> localMap;
    char localMapBuffer[2048];
    hkPointerMapBase *map;
    hkInplaceArray<ParentCount, 8> parents; /* {data,size,cap} triple + 8 inline slots */
    int numParents;
    int i, j, k;

    hkPointerMapBase_construct(&localMap.m_map, (hkPointerMapPair *)localMapBuffer, 2048); /* buffer is raw Pair storage */
    map = shapesMapOut ? &((hkPointerMap<const hkdBreakableShape *, int> *)shapesMapOut)->m_map
                       : &localMap.m_map;

    parents.m_data = parents.m_storage;
    parents.m_size = 0;
    parents.m_capacityAndFlags = (int)0x80000008; /* inline capacity 8, not heap-owned */

    for (i = 0; i < numShapesIn; ++i)
        hkPointerMapBase_insert(map, (unsigned int)shapesIn[i].m_shape, (unsigned int)i);

    for (i = 0; i < numShapesIn; ++i)
    {
        const hkdBreakableShape *parent = shapesIn[i].m_shape->m_parent;
        while (parent)
        {
            int found = 0;
            for (j = 0; j < parents.m_size; ++j)
            {
                if (parents.m_data[j].m_parent == parent)
                {
                    ++parents.m_data[j].m_count;
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                if (parents.m_size == (parents.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&parents.m_data, (int)sizeof(ParentCount));
                parents.m_data[parents.m_size].m_parent = parent;
                parents.m_data[parents.m_size].m_count = 1;
                ++parents.m_size;
            }
            parent = parent->m_parent;
        }
    }

    numParents = parents.m_size;
    for (i = 0; i < numParents; ++i)
    {
        const hkdBreakableShape *parent = parents.m_data[i].m_parent;
        if (parents.m_data[i].m_count == 1)
            continue; /* referenced by only one leaf: no in-set connections */
        for (k = 0; k < parent->m_connections.m_size; ++k)
        {
            const hkdBreakableShape_Connection *conn = &parent->m_connections.m_data[k];
            int idxA = hkPointerMapBase_findKey(map, (unsigned int)conn->m_a);
            if (idxA <= map->m_hashMod)
            {
                int idxB = hkPointerMapBase_findKey(map, (unsigned int)conn->m_b);
                if (idxB <= map->m_hashMod)
                {
                    hkdControllerUtil_ConnectionRef *ref;
                    if (connectionsOut->m_size == (connectionsOut->m_capacityAndFlags & 0x3FFFFFFF))
                        hkArrayUtil__reserveMore(&connectionsOut->m_data, (int)sizeof(hkdControllerUtil_ConnectionRef));
                    ref = &connectionsOut->m_data[connectionsOut->m_size++];
                    ref->m_connection = conn;
                    ref->m_idA = (unsigned short)map->m_elem[idxA].val;
                    ref->m_idB = (unsigned short)map->m_elem[idxB].val;
                }
            }
        }
    }

    if ((parents.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), parents.m_data,
                                       (int)sizeof(ParentCount) * (parents.m_capacityAndFlags & 0x3FFFFFFF),
                                       HK_MEMORY_CLASS_ARRAY);
    hkPointerMapBase_destruct(&localMap.m_map);
}
