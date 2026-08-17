/* ?buildConvexSubHull@hkGeomConvexHullBuilder@@SAXABUhkGeomConvexHullTolerances@@AAV?$hkArray@VhkVector4@@@@HHAAVhkGeomHull@@@Z @0x83FDE488 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* Append every edge of src->m_edges onto dst->m_edges, growing dst as needed.
   Reconstructed from the two identical inline copy loops in the decompiler. */
static void hkGeomConvexHullBuilder_appendEdges(hkGeomHull *dst, hkGeomHull *src)
{
    int k;
    for (k = 0; k < src->m_edges.m_size; ++k)
    {
        hkGeomEdge *out;
        if (dst->m_edges.m_size == (dst->m_edges.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&dst->m_edges.m_data, 8);
        out = &dst->m_edges.m_data[dst->m_edges.m_size];
        *out = src->m_edges.m_data[k];
        ++dst->m_edges.m_size;
    }
}

/* hkGeomConvexHullBuilder::buildConvexSubHull
   Divide-and-conquer builder over the x-sorted vertex slice [startVertex, endVertex]. Counts
   the still-active vertices (w-lane == 0.0). With <= 3 active vertices it emits a trivial hull
   (point / edge / triangle) directly. Otherwise it splits at the midpoint, recursively builds
   the left and right sub-hulls into scratch hulls (iterating postFilterVertices until each is
   stable, then again over the full range until neither changes), and merges the two halves via
   mergeHulls (or, if one half is empty, copies the non-empty half's edges into the output). */
void hkGeomConvexHullBuilder_buildConvexSubHull(const hkGeomConvexHullTolerances *tolerances,
        hkArray<hkVector4> *xSortedVerts, int startVertex, int endVertex, hkGeomHull *hullOut)
{
    int rangeEnd = endVertex + 1;
    int activeCount = 0;
    int i;

    /* count active (unflagged) vertices in [startVertex, endVertex] */
    for (i = startVertex; i < rangeEnd; ++i)
    {
        if (xSortedVerts->m_data[i].m_quad.___u0.v[3] == 0.0f)
            ++activeCount;
    }

    if (activeCount <= 3)
    {
        /* ---- leaf: gather active indices (at most 3) and build a trivial hull ---- */
        int indexList[4];
        int listSize = 0;

        hullOut->m_vertexBase = xSortedVerts->m_data;

        for (i = startVertex; i < rangeEnd; ++i)
        {
            if (xSortedVerts->m_data[i].m_quad.___u0.v[3] == 0.0f)
                indexList[listSize++] = i; /* capacity 3; never exceeded when activeCount<=3 */
        }

        if (activeCount == 1)
            hkGeomHull_initializeWithVertex(hullOut, indexList[0]);
        else if (activeCount == 2)
            hkGeomHull_initializeWithEdge(hullOut, indexList[0], indexList[1]);
        else if (activeCount >= 3)
            hkGeomHull_initializeWithTriangle(hullOut, indexList[0], indexList[1], indexList[2]);
        /* activeCount == 0: empty hull */
        return;
    }

    /* ---- split ---- */
    {
        int mid = (startVertex + endVertex) / 2;
        hkGeomHull leftHull;
        hkGeomHull rightHull;
        hkBool leftChanged, rightChanged;
        hkThreadMemory *mem;

        hkGeomHull_construct(&rightHull);
        rightHull.m_vertexBase = xSortedVerts->m_data;
        hkGeomHull_construct(&leftHull);
        leftHull.m_vertexBase = xSortedVerts->m_data;

        do
        {
            leftChanged.m_bool = 1;
            do
            {
                leftHull.m_edges.m_size = 0;
                hkGeomConvexHullBuilder_buildConvexSubHull(tolerances, xSortedVerts,
                        startVertex, (startVertex + endVertex) / 2, &leftHull);
                hkGeomConvexHullBuilder_postFilterVertices(&leftHull, startVertex,
                        (startVertex + endVertex) / 2, tolerances, &leftChanged);
            } while (leftChanged.m_bool);

            leftChanged.m_bool = 1;
            do
            {
                rightHull.m_edges.m_size = 0;
                hkGeomConvexHullBuilder_buildConvexSubHull(tolerances, xSortedVerts,
                        mid + 1, endVertex, &rightHull);
                hkGeomConvexHullBuilder_postFilterVertices(&rightHull, mid + 1, endVertex,
                        tolerances, &leftChanged);
            } while (leftChanged.m_bool);

            leftChanged.m_bool = 1;
            hkGeomConvexHullBuilder_postFilterVertices(&leftHull, startVertex, endVertex,
                    tolerances, &leftChanged);
            rightChanged.m_bool = 1;
            hkGeomConvexHullBuilder_postFilterVertices(&rightHull, startVertex, endVertex,
                    tolerances, &rightChanged);
        } while (leftChanged.m_bool || rightChanged.m_bool);

        if (leftHull.m_edges.m_size)
        {
            if (rightHull.m_edges.m_size)
                hkGeomConvexHullBuilder_mergeHulls(tolerances, &leftHull, &rightHull, hullOut);
            else
                hkGeomConvexHullBuilder_appendEdges(hullOut, &leftHull);
        }
        else
        {
            hkGeomConvexHullBuilder_appendEdges(hullOut, &rightHull);
        }

        if (!hullOut->m_edges.m_size)
            hkInplaceArray_hkGeomEdge_128_assign(&hullOut->m_edges, &leftHull.m_edges);

        /* release scratch hull edge buffers (only when spilled to the heap) */
        mem = hkThreadMemory_getCurrent();
        if (rightHull.m_edges.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(mem, rightHull.m_edges.m_data,
                    8 * rightHull.m_edges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
        if (leftHull.m_edges.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(mem, leftHull.m_edges.m_data,
                    8 * leftHull.m_edges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    }
}
