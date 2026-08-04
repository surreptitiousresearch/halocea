#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkGeomHull.h"
#include "../headers/havok/hkFreeList.h"
#include "../headers/havok/hkpConvexVerticesConnectivity.h"
#include "../headers/havok/hkThreadMemory.h"
#include <math.h>

extern void hkArrayUtil__reserve(void *arrayData, int newCapacity, int sizeElem);  /* arg0 void* per DB (PAX); synced to match the call sites 2026-07-31 */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* hkpConvexVerticesConnectivity's vtable (boundary symbol). */
extern void *hkpConvexVerticesConnectivity_vftable;

/* Memory-class tag for the connectivity allocation (name from the DB; value is an
   SDK-floor enum left extern). */
extern const int HK_MEMORY_CLASS_CDINFO;

/* One directed face edge (12-byte hkFreeList element): to-vertex, from-vertex and
   the intra-face list link. */
typedef struct ConnEdgeNode
{
    int m_to;                    /* 0 */
    int m_from;                  /* 4 */
    struct ConnEdgeNode *m_next; /* 8 */
} ConnEdgeNode;

/* squared 3D distance between an hkVector4 and another. */
static float conn_dist2(const hkVector4 *a, const hkVector4 *b)
{
    float dx = a->m_quad.___u0.v[0] - b->m_quad.___u0.v[0];
    float dy = a->m_quad.___u0.v[1] - b->m_quad.___u0.v[1];
    float dz = a->m_quad.___u0.v[2] - b->m_quad.___u0.v[2];
    return dx*dx + dy*dy + dz*dz;
}

/* _findConnectivity  (?_findConnectivity@@YAPAVhkpConvexVerticesConnectivity@@...@Z)
   Builds the face/vertex adjacency object for a convex-vertices shape from its input
   vertices, its face plane equations, the welded "used" vertices and the geometric
   hull.

   Algorithm:
     1. Map every used vertex back to the closest original input vertex (identity when
        the used array *is* the input array).
     2. Walk each hull face loop (marking visited edges), collect its vertices, pick
        the best-matching face plane (smallest max |plane.dot(v)| over the loop), and
        push each consecutive directed edge into that plane's per-face linked list
        (pooled through an hkFreeList of 12-byte nodes).
     3. Allocate the connectivity object; for every face, turn its directed-edge list
        into a successor map, walk the resulting cycle and emit the remapped vertex
        indices plus the per-face vertex count.

   DEVIATION: the VMX distance/normalise search (vmsum3fp + rsqrt + comparison masks)
   is reduced to a scalar nearest-point search; the inline stack-backed scratch arrays
   (sign-bit DONT_DEALLOCATE capacities) are modelled as ordinary growable hkArrays
   freed on exit; the reverse-edge de-duplication pass is expressed as a successor-map
   build (equivalent for well-formed convex faces). */
hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil__findConnectivity_impl(
        const hkArray<hkVector4> *vertices, const hkArray<hkVector4> *facePlanes,
        const hkArray<hkVector4> *usedVertices, hkGeomHull *hull)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int numUsed = usedVertices->m_size;
    int numEdges = hull->m_edges.m_size;
    int numFaces = facePlanes->m_size;
    int numInput = vertices->m_size;

    hkArray<int> vremap = { nullptr, 0, (int)0x80000000 };
    hkArray<char> edgeVisited = { nullptr, 0, (int)0x80000000 };
    hkArray<ConnEdgeNode *> faceHeads = { nullptr, 0, (int)0x80000000 };
    hkArray<int> loopVerts = { nullptr, 0, (int)0x80000000 };
    hkArray<int> successor = { nullptr, 0, (int)0x80000000 };
    hkFreeList freeList;
    hkpConvexVerticesConnectivity *conn;
    int e, f, i;

    /* ---- 1: used-vertex -> input-vertex remap ---- */
    if (numUsed > 0) hkArrayUtil__reserve(&vremap.m_data, numUsed, 4);
    vremap.m_size = numUsed;
    if (vertices->m_data != usedVertices->m_data || numInput != numUsed)
    {
        for (i = 0; i < numUsed; ++i)
        {
            int best = -1;
            float bestD = 1.0e10f;
            int k;
            for (k = 0; k < numInput; ++k)
            {
                float d = conn_dist2(&usedVertices->m_data[i], &vertices->m_data[k]);
                if (d < bestD) { bestD = d; best = k; }
            }
            vremap.m_data[i] = best;
        }
    }
    else
    {
        for (i = 0; i < numUsed; ++i)
            vremap.m_data[i] = i;
    }

    /* ---- 2: per-face directed edge lists ---- */
    if (numEdges > 0) hkArrayUtil__reserve(&edgeVisited.m_data, numEdges, 1);
    edgeVisited.m_size = numEdges;
    for (i = 0; i < numEdges; ++i)
        edgeVisited.m_data[i] = 0;

    if (numFaces > 0) hkArrayUtil__reserve(&faceHeads.m_data, numFaces, 4);
    faceHeads.m_size = numFaces;
    for (i = 0; i < numFaces; ++i)
        faceHeads.m_data[i] = nullptr;

    hkFreeList_construct(&freeList, 12u, 4u, 0x800u, hkDefaultFreeListMemoryServer_getInstance());

    for (e = 0; e < numEdges; ++e)
    {
        if (edgeVisited.m_data[e])
            continue;

        /* walk the face loop */
        loopVerts.m_size = 0;
        {
            hkGeomEdge *start = &hull->m_edges.m_data[e];
            hkGeomEdge *cur = start;
            do
            {
                if (loopVerts.m_size == (loopVerts.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&loopVerts.m_data, 4);
                loopVerts.m_data[loopVerts.m_size++] = cur->m_vertex;
                edgeVisited.m_data[cur->m_next] = 1;
                cur = &hull->m_edges.m_data[cur->m_next];
            }
            while (cur != start);
        }

        /* choose the best-matching face plane */
        f = -1;
        if (loopVerts.m_size > 0)
        {
            float bestFace = 1.0e10f;
            int pf;
            for (pf = 0; pf < numFaces; ++pf)
            {
                const hkVector4 *pl = &facePlanes->m_data[pf];
                float maxDist = 0.0f;
                int li;
                for (li = 0; li < loopVerts.m_size; ++li)
                {
                    const hkVector4 *v = &usedVertices->m_data[loopVerts.m_data[li]];
                    float d = fabsf(pl->m_quad.___u0.v[0]*v->m_quad.___u0.v[0]
                                  + pl->m_quad.___u0.v[1]*v->m_quad.___u0.v[1]
                                  + pl->m_quad.___u0.v[2]*v->m_quad.___u0.v[2]
                                  + pl->m_quad.___u0.v[3]);
                    if (d > maxDist) maxDist = d;
                }
                if (maxDist < bestFace) { bestFace = maxDist; f = pf; }
            }
        }

        /* push directed edges (from previous vertex to current) into face f's list */
        if (loopVerts.m_size > 0 && f >= 0)
        {
            int prev = loopVerts.m_data[loopVerts.m_size - 1];
            int li;
            for (li = 0; li < loopVerts.m_size; ++li)
            {
                int curV = loopVerts.m_data[li];
                ConnEdgeNode *node = (ConnEdgeNode *)hkFreeList_alloc(&freeList);
                node->m_from = prev;
                node->m_to = curV;
                node->m_next = faceHeads.m_data[f];
                faceHeads.m_data[f] = node;
                prev = curV;
            }
        }
    }

    /* ---- 3: allocate the connectivity object and emit indices ---- */
    conn = (hkpConvexVerticesConnectivity *)hkThreadMemory_allocateChunk(mem, 32, HK_MEMORY_CLASS_CDINFO);
    conn->base.__vftable = (hkBaseObject_vtbl *)&hkpConvexVerticesConnectivity_vftable;
    conn->base.m_memSizeAndFlags = 32;
    conn->base.m_referenceCount = 1;
    conn->m_vertexIndices.m_data = nullptr;
    conn->m_vertexIndices.m_size = 0;
    conn->m_vertexIndices.m_capacityAndFlags = (int)0x80000000;
    conn->m_numVerticesPerFace.m_data = nullptr;
    conn->m_numVerticesPerFace.m_size = 0;
    conn->m_numVerticesPerFace.m_capacityAndFlags = (int)0x80000000;

    if (numInput > 0) hkArrayUtil__reserve(&successor.m_data, numInput, 4);
    successor.m_size = numInput;

    for (f = 0; f < numFaces; ++f)
    {
        ConnEdgeNode *node = faceHeads.m_data[f];
        int startVert, cur, before;

        if (!node)
        {
            /* empty face -> zero-length loop */
            if (conn->m_numVerticesPerFace.m_size == (conn->m_numVerticesPerFace.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&conn->m_numVerticesPerFace.m_data, 1);
            conn->m_numVerticesPerFace.m_data[conn->m_numVerticesPerFace.m_size++] = 0;
            continue;
        }

        /* successor map: to-vertex -> from-vertex, and remember a start vertex */
        for (; node; node = node->m_next)
            successor.m_data[node->m_to] = node->m_from;
        startVert = faceHeads.m_data[f]->m_to;

        before = conn->m_vertexIndices.m_size;
        cur = startVert;
        do
        {
            unsigned short idx = (unsigned short)vremap.m_data[cur];
            if (conn->m_vertexIndices.m_size == (conn->m_vertexIndices.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&conn->m_vertexIndices.m_data, 2);
            conn->m_vertexIndices.m_data[conn->m_vertexIndices.m_size++] = idx;
            cur = successor.m_data[cur];
        }
        while (cur != startVert);

        if (conn->m_numVerticesPerFace.m_size == (conn->m_numVerticesPerFace.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&conn->m_numVerticesPerFace.m_data, 1);
        conn->m_numVerticesPerFace.m_data[conn->m_numVerticesPerFace.m_size++] =
            (unsigned char)(conn->m_vertexIndices.m_size - before);
    }

    /* ---- free scratch ---- */
    if (successor.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, successor.m_data, 4 * (successor.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
    hkFreeList_freeAllMemory(&freeList);
    if (loopVerts.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, loopVerts.m_data, 4 * (loopVerts.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
    if (faceHeads.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, faceHeads.m_data, 4 * (faceHeads.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
    if (edgeVisited.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, edgeVisited.m_data, edgeVisited.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
    if (vremap.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, vremap.m_data, 4 * (vremap.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    return conn;
}
