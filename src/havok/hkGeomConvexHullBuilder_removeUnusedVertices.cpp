#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* Per-thread scratch (bump) stack allocator. DEVIATION: the original inlines the
   hkThreadMemory::Stack bump pointer (m_stack.m_current / m_stack.m_end with vtable
   overflow slot +20 and underflow slot +24); modelled here as boundary calls so the
   allocator stays at the SDK floor. */
extern void *hkThreadMemory_allocateStack(hkThreadMemory *self, int numBytes);
extern void hkThreadMemory_deallocateStack(hkThreadMemory *self, void *ptr);

/* hkGeomConvexHullBuilder::removeUnusedVertices
   Drops vertices not referenced by any hull edge and renumbers the edges' vertex indices to
   match the compacted vertex array.
     1. Flag every referenced vertex (w-lane = 2.0).
     2. Stream-compact the used vertices to the front, building a remap[old] -> new (or -1).
     3. setSize to the number of used vertices.
     4. Rewrite each edge's m_vertex through the remap table.
     5. Clear the w-lane flags on the surviving vertices. */
void hkGeomConvexHullBuilder_removeUnusedVertices(hkGeomHull *hull, hkArray<hkVector4> *verts)
{
    hkThreadMemory *mem;
    int *remap;
    int allocBytes;
    int i, newCount, cap;

    /* 1. mark referenced vertices */
    for (i = 0; i < hull->m_edges.m_size; ++i)
        verts->m_data[hull->m_edges.m_data[i].m_vertex].m_quad.___u0.v[3] = 2.0f;

    /* 2. allocate remap[] on the thread scratch stack (padded/aligned as the original) */
    mem = hkThreadMemory_getCurrent();
    allocBytes = (4 * (verts->m_size + 4)) & ~15;
    remap = (int *)hkThreadMemory_allocateStack(mem, allocBytes);

    newCount = 0;
    for (i = 0; i < verts->m_size; ++i)
    {
        if (verts->m_data[i].m_quad.___u0.v[3] == 2.0f)
        {
            verts->m_data[newCount] = verts->m_data[i];
            remap[i] = newCount++;
        }
        else
        {
            remap[i] = -1;
        }
    }

    /* 3. shrink the vertex array to the used count */
    cap = verts->m_capacityAndFlags & 0x3FFFFFFF;
    if (cap < newCount)
    {
        int grow = 2 * cap;
        if (newCount >= grow)
            grow = newCount;
        hkArrayUtil__reserve(&verts->m_data, grow, 16);
    }
    verts->m_size = newCount;

    /* 4. remap edge vertex indices */
    for (i = 0; i < hull->m_edges.m_size; ++i)
    {
        hkGeomEdge *e = &hull->m_edges.m_data[i];
        e->m_vertex = (unsigned short)remap[e->m_vertex];
    }

    /* free scratch (restores the bump pointer) */
    hkThreadMemory_deallocateStack(mem, remap);

    /* 5. clear the w-lane flags using the new indices */
    for (i = 0; i < hull->m_edges.m_size; ++i)
        verts->m_data[hull->m_edges.m_data[i].m_vertex].m_quad.___u0.v[3] = 0.0f;
}
