#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkStridedVertices.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkPointerMapBase.h"
#include "../headers/havok/hkReferencedObject.h"

/* ----- boundary callees (2-level descent limit) ----- */
extern void hkArrayUtil__reserveMore(void *arrayData, int elementSize);          /* hkArrayUtil::_reserveMore */
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

extern void hkPointerMapBase_construct_default(hkPointerMapBase *self);          /* hkPointerMapBase::hkPointerMapBase() */
extern void hkPointerMapBase_reserve(hkPointerMapBase *self, int numElems);     /* hkPointerMapBase::reserve */

extern const int HK_MEMORY_CLASS_CDINFO;

/* vtable symbol for the on-demand connectivity object. */
extern hkBaseObject_vtbl hkpConvexVerticesConnectivity_vftable;                 /* hkpConvexVerticesConnectivity::`vftable' */

/* Per-original-vertex classification record (12 bytes), indexed by original
   vertex id. outIndex = its slot in the cut-vertex array (valid when inside);
   flag = 1 when the vertex is on the cut-away (positive) side of the plane, 0 when
   it survives; dist = signed distance to the (radius-offset) cut plane. */
typedef struct CutVertexClass
{
    int outIndex; /* 0 */
    int flag;     /* 4 */
    float dist;   /* 8 */
} CutVertexClass;

/* Open boundary edge produced while clipping a face (8 bytes). */
typedef struct OpenEdge
{
    int a; /* 0 */
    int b; /* 4 */
} OpenEdge;

/* Copy one hkVector4 (VMX lvx/stvx pair reconstructed as scalar). */
static void copyVector4(hkVector4 *dst, const hkVector4 *src)
{
    dst->m_quad.___u0.v[0] = src->m_quad.___u0.v[0];
    dst->m_quad.___u0.v[1] = src->m_quad.___u0.v[1];
    dst->m_quad.___u0.v[2] = src->m_quad.___u0.v[2];
    dst->m_quad.___u0.v[3] = src->m_quad.___u0.v[3];
}

/* Append one plane/vector to a stack-or-inplace hkVector4 array, growing on demand. */
static void appendVector4(hkArray<hkVector4> *arr, const hkVector4 *value)
{
    int index = arr->m_size;
    if (index == (arr->m_capacityAndFlags & 0x3FFFFFFF))
    {
        hkArrayUtil__reserveMore(&arr->m_data, 16);
        index = arr->m_size;
    }
    copyVector4(&arr->m_data[index], value);
    arr->m_size = index + 1;
}

static void appendVertexIndex(hkArray<unsigned short> *arr, unsigned short value)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 2);
    arr->m_data[arr->m_size++] = value;
}

static void appendFaceVertCount(hkArray<unsigned char> *arr, unsigned char value)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 1);
    arr->m_data[arr->m_size++] = value;
}

/* hkpConvexVerticesConnectivityUtil::cut
   Cuts a convex hull with a plane (offset outward by convexRadius) and returns a
   newly allocated convex-vertices shape for the surviving (negative) half, with
   rebuilt connectivity. Returns the original shape (with an added reference) when
   nothing is removed, and null when the whole hull is removed or the remainder is
   below minVolume.

   DEVIATION: the real signature is
     static const hkpConvexVerticesShape* cut(const hkpConvexVerticesShape*,
                                              const hkVector4&, float, float)
   (mangled ?cut@hkpConvexVerticesConnectivityUtil@@SAPBV...ABVhkVector4@@MM@Z);
   the decompiler invented eleven trailing integer args from unused registers.
   All VMX128 math is reconstructed as scalar over the four float lanes, and the
   inlined per-thread bump/pop allocator is expressed through the hkThreadMemory
   stack API. */
const hkpConvexVerticesShape *hkpConvexVerticesConnectivityUtil_cut(
        hkpConvexVerticesShape *shape,
        const hkVector4 *plane,
        float convexRadius,
        float minVolume)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    const hkpConvexVerticesConnectivity *connectivity = shape->m_connectivity;
    int numSpheres = hkpConvexVerticesShape_getNumCollisionSpheresImpl(shape);

    hkVector4 cutPlane;              /* radius-offset plane used for classification */
    hkArray<hkVector4> originalVertices;
    hkVector4 *origVertsBase;
    CutVertexClass *vinfo;
    const hkArray<hkVector4> *planeEquations;
    hkArray<hkVector4> cutVertices;
    hkVector4 *cutVertsBase;
    int numVerts;
    int i;

    /* Offset the plane outward by the convex radius. */
    copyVector4(&cutPlane, plane);
    cutPlane.m_quad.___u0.v[3] += convexRadius;

    originalVertices.m_data = (hkVector4 *)hkThreadMemory_allocateStack(mem, (numSpheres + 1) * 16);
    originalVertices.m_size = 0;
    originalVertices.m_capacityAndFlags = numSpheres | 0x80000000;
    origVertsBase = originalVertices.m_data;
    hkpConvexVerticesShape_getOriginalVertices(shape, &originalVertices);
    numVerts = originalVertices.m_size;

    vinfo = (CutVertexClass *)hkThreadMemory_allocateStack(mem, (12 * numVerts + 16) & 0xFFFFFFF0);

    planeEquations = hkpConvexVerticesShape_getPlaneEquations(shape);

    cutVertices.m_data = (hkVector4 *)hkThreadMemory_allocateStack(mem, (numVerts + 1) * 16);
    cutVertices.m_size = 0;
    cutVertices.m_capacityAndFlags = numVerts | 0x80000000;
    cutVertsBase = cutVertices.m_data;

    /* Classify each original vertex against the cut plane; keep the negative side. */
    for (i = 0; i < numVerts; ++i)
    {
        const hkVector4 *v = &originalVertices.m_data[i];
        float dist = cutPlane.m_quad.___u0.v[0] * v->m_quad.___u0.v[0]
                   + cutPlane.m_quad.___u0.v[1] * v->m_quad.___u0.v[1]
                   + cutPlane.m_quad.___u0.v[2] * v->m_quad.___u0.v[2]
                   + cutPlane.m_quad.___u0.v[3];
        vinfo[i].dist = dist;
        if (dist > 0.0f)
        {
            vinfo[i].flag = 1;
        }
        else
        {
            vinfo[i].flag = 0;
            vinfo[i].outIndex = cutVertices.m_size;
            copyVector4(&cutVertices.m_data[cutVertices.m_size++], v);
        }
    }

    if (cutVertices.m_size == 0)
    {
        /* Whole hull is on the cut-away side. */
        hkThreadMemory_deallocateStack(mem, cutVertsBase);
        hkThreadMemory_deallocateStack(mem, vinfo);
        hkThreadMemory_deallocateStack(mem, origVertsBase);
        return nullptr;
    }

    if (cutVertices.m_size == numVerts)
    {
        /* Nothing removed — hand back the original shape with a new reference. */
        hkReferencedObject_addReference((hkReferencedObject *)&shape->base);
        hkThreadMemory_deallocateStack(mem, cutVertsBase);
        hkThreadMemory_deallocateStack(mem, vinfo);
        hkThreadMemory_deallocateStack(mem, origVertsBase);
        return shape;
    }

    /* ----- genuine cut: rebuild vertices, faces and connectivity ----- */
    {
        hkpConvexVerticesConnectivity *newConn =
            (hkpConvexVerticesConnectivity *)hkThreadMemory_allocateChunk(mem, 32, HK_MEMORY_CLASS_CDINFO);
        hkArray<unsigned short> *outVertexIndices = &newConn->m_vertexIndices;
        hkArray<unsigned char> *outNumVertsPerFace = &newConn->m_numVerticesPerFace;

        hkVector4 facePlanesInplace[32];
        hkArray<hkVector4> newFacePlanes;

        OpenEdge edgesInplace[32];
        OpenEdge *edges = edgesInplace;
        int edgeCount = 0;
        int edgeCapFlags = (int)0x80000020;   /* inplace, capacity 32 */

        hkPointerMapBase edgeMap;

        int faceCount = connectivity->m_numVerticesPerFace.m_size;
        int connBase = 0;      /* running offset into connectivity->m_vertexIndices */
        int faceIdx;
        int remainingEdges;

        const hkpConvexVerticesConnectivity *finalConn;

        newConn->base.m_referenceCount = 1;
        newConn->base.__vftable = &hkpConvexVerticesConnectivity_vftable;
        newConn->base.m_memSizeAndFlags = 32;
        newConn->m_vertexIndices.m_data = nullptr;
        newConn->m_vertexIndices.m_size = 0;
        newConn->m_vertexIndices.m_capacityAndFlags = (int)0x80000000;
        newConn->m_numVerticesPerFace.m_data = nullptr;
        newConn->m_numVerticesPerFace.m_size = 0;
        newConn->m_numVerticesPerFace.m_capacityAndFlags = (int)0x80000000;

        newFacePlanes.m_data = facePlanesInplace;
        newFacePlanes.m_size = 0;
        newFacePlanes.m_capacityAndFlags = (int)0x80000020;

        hkPointerMapBase_construct_default(&edgeMap);
        hkPointerMapBase_reserve(&edgeMap, 2 * numVerts);

        for (faceIdx = 0; faceIdx < faceCount; ++faceIdx)
        {
            const unsigned char *numVPF = connectivity->m_numVerticesPerFace.m_data;
            int nInside = 0;
            int lastInsideLocal = -1;
            int faceVertCount = numVPF[faceIdx];
            int savedVIdxSize = outVertexIndices->m_size;
            int localIdx;
            int k;

            /* Count how many of this face's vertices survive the cut. */
            for (localIdx = 0, k = 0; k < faceVertCount; ++k, ++localIdx)
            {
                unsigned short gvid = connectivity->m_vertexIndices.m_data[connBase + k];
                if (vinfo[gvid].flag == 0)
                {
                    lastInsideLocal = localIdx;
                    ++nInside;
                }
            }

            if (nInside == faceVertCount)
            {
                /* Entire face survives: copy it verbatim (remapped indices). */
                for (k = 0; k < faceVertCount; ++k)
                {
                    unsigned short gvid = connectivity->m_vertexIndices.m_data[connBase + k];
                    appendVertexIndex(outVertexIndices, (unsigned short)vinfo[gvid].outIndex);
                }
                appendFaceVertCount(outNumVertsPerFace, (unsigned char)faceVertCount);
                appendVector4(&newFacePlanes, &planeEquations->m_data[faceIdx]);
            }
            else if (lastInsideLocal >= 0)
            {
                /* Face straddles the plane: clip its edge loop. */
                int savedVIdxSizeThisFace = savedVIdxSize;
                int prevGid = connectivity->m_vertexIndices.m_data[lastInsideLocal + connBase];
                int walk = lastInsideLocal;
                int pendingOpen = -1;   /* previous open-crossing vertex index (v94) */
                int addedThisFace;

                for (k = 0; k < faceVertCount; ++k)
                {
                    int curGid;
                    CutVertexClass *infoPrev;
                    CutVertexClass *infoCur;

                    if (walk == faceVertCount - 1)
                        walk = 0;
                    else
                        ++walk;

                    curGid = connectivity->m_vertexIndices.m_data[walk + connBase];
                    infoPrev = &vinfo[prevGid];
                    infoCur = &vinfo[curGid];

                    if (infoCur->flag != infoPrev->flag)
                    {
                        /* Edge crosses the plane: get (or create) the split vertex. */
                        unsigned int edgeKey;
                        unsigned int splitVal;
                        unsigned int existing;

                        if (prevGid >= curGid)
                            edgeKey = ((unsigned int)curGid << 16) | (unsigned int)prevGid;
                        else
                            edgeKey = ((unsigned int)prevGid << 16) | (unsigned int)curGid;

                        existing = hkPointerMapBase_findKey(&edgeMap, edgeKey);
                        if ((int)existing > edgeMap.m_hashMod)
                        {
                            /* Not seen before: interpolate a new vertex on the plane. */
                            float distPrev = infoPrev->dist;
                            float distCur = infoCur->dist;
                            float t = distPrev / (distPrev - distCur);
                            const hkVector4 *va = &originalVertices.m_data[prevGid];
                            const hkVector4 *vb = &originalVertices.m_data[curGid];
                            int newIdx = cutVertices.m_size;
                            hkVector4 interp;
                            int lane;

                            for (lane = 0; lane < 4; ++lane)
                            {
                                float a = va->m_quad.___u0.v[lane];
                                float b = vb->m_quad.___u0.v[lane];
                                interp.m_quad.___u0.v[lane] = a + t * (b - a);
                            }
                            appendVector4(&cutVertices, &interp);
                            hkPointerMapBase_insert(&edgeMap, edgeKey, (unsigned int)newIdx);
                            splitVal = (unsigned int)newIdx;
                        }
                        else
                        {
                            splitVal = edgeMap.m_elem[existing].val;
                        }

                        if (infoPrev->flag)
                        {
                            /* Leaving the inside region: emit the split vertex and
                               open a boundary edge from the last open crossing. */
                            appendVertexIndex(outVertexIndices, (unsigned short)splitVal);
                            if (edgeCount == (edgeCapFlags & 0x3FFFFFFF))
                            {
                                hkArrayUtil__reserveMore(&edges, 8);
                                /* DEVIATION: reserveMore may relocate the inplace edge
                                   buffer; capacity flag loses its inplace bit. */
                                edgeCapFlags = edgeCount | 0x40000000;
                            }
                            /* DEVIATION: the decompiler renders this qword store with
                               the byte-offset expression leaking into the high word;
                               the intent is to record the open edge (pendingOpen -> split). */
                            edges[edgeCount].a = pendingOpen;
                            edges[edgeCount].b = (int)splitVal;
                            ++edgeCount;
                            pendingOpen = -1;
                        }
                        else
                        {
                            /* Entering the inside region: keep the surviving endpoint
                               then the split vertex; remember the split as pending. */
                            appendVertexIndex(outVertexIndices, (unsigned short)infoPrev->outIndex);
                            appendVertexIndex(outVertexIndices, (unsigned short)splitVal);
                            pendingOpen = (int)splitVal;
                        }
                    }
                    else if (infoPrev->flag == 0)
                    {
                        /* Interior edge, both endpoints survive. */
                        appendVertexIndex(outVertexIndices, (unsigned short)infoPrev->outIndex);
                    }

                    prevGid = curGid;
                }

                addedThisFace = outVertexIndices->m_size - savedVIdxSizeThisFace;
                if (outVertexIndices->m_size != savedVIdxSizeThisFace)
                {
                    if (addedThisFace > 2)
                    {
                        appendVector4(&newFacePlanes, &planeEquations->m_data[faceIdx]);
                        appendFaceVertCount(outNumVertsPerFace, (unsigned char)addedThisFace);
                    }
                    else
                    {
                        /* Degenerate remnant: roll the face's indices back. */
                        int cap = outVertexIndices->m_capacityAndFlags & 0x3FFFFFFF;
                        if (cap < savedVIdxSizeThisFace)
                        {
                            int want = 2 * cap;
                            if (savedVIdxSizeThisFace >= want)
                                want = savedVIdxSizeThisFace;
                            hkArrayUtil__reserve(&outVertexIndices->m_data, want, 2);
                        }
                        outVertexIndices->m_size = savedVIdxSizeThisFace;
                    }
                }
            }

            connBase += faceVertCount;
        }

        /* Close the newly exposed cut face from the accumulated open edges. */
        remainingEdges = edgeCount;
        if (edgeCount >= 3)
        {
            do
            {
                int savedVIdxSize = outVertexIndices->m_size;
                int startA = edges[0].a;
                int startB = edges[0].b;
                unsigned short firstVert = (unsigned short)edges[0].a;
                unsigned char faceVerts;

                --remainingEdges;
                edges[0] = edges[remainingEdges];     /* swap-remove edge 0 */
                appendVertexIndex(outVertexIndices, firstVert);

                while (startA != startB)
                {
                    int found = 0;
                    int closeLoop = 0;

                    if (remainingEdges <= 0)
                    {
                        closeLoop = 1;
                    }
                    else
                    {
                        OpenEdge *scan = edges;
                        while (scan->a != startB)
                        {
                            ++found;
                            ++scan;
                            if (found >= remainingEdges)
                            {
                                closeLoop = 1;
                                break;
                            }
                        }
                        if (!closeLoop)
                        {
                            appendVertexIndex(outVertexIndices, (unsigned short)startB);
                            startB = scan->b;
                            --remainingEdges;
                            edges[found] = edges[remainingEdges];   /* swap-remove */
                        }
                    }

                    if (closeLoop)
                    {
                        int sz;
                        if (outVertexIndices->m_size == 0)
                            break;   /* START_NEW_EDGE */
                        sz = outVertexIndices->m_size;
                        startB = outVertexIndices->m_data[sz - 1];
                        outVertexIndices->m_size = sz - 1;
                    }
                }

                faceVerts = (unsigned char)(outVertexIndices->m_size - savedVIdxSize);
                appendFaceVertCount(outNumVertsPerFace, faceVerts);
                appendVector4(&newFacePlanes, plane);   /* the (un-offset) cut plane */

                remainingEdges = remainingEdges;
            }
            while (remainingEdges >= 3);
        }

        finalConn = newConn;

        if (hkpConvexVerticesConnectivityUtil_calculateVolume(&cutVertices, newConn) >= minVolume)
        {
            hkStridedVertices strided;
            hkpConvexVerticesShape *result;

            strided.m_striding = 16;
            strided.m_vertices = cutVertices.m_data->m_quad.___u0.vector4_f32; /* union view name per hkVector4/__vector4 DB layout */
            strided.m_numVertices = cutVertices.m_size;

            result = (hkpConvexVerticesShape *)hkThreadMemory_allocateChunk(mem, 96, HK_MEMORY_CLASS_CDINFO);
            result->base.m_memSizeAndFlags = 96;
            hkpConvexVerticesShape_construct(result, &strided, &newFacePlanes, shape->m_radius);
            hkpConvexVerticesShape_setConnectivity(result, finalConn);
            result->m_radius = shape->m_radius;
            hkReferencedObject_removeReference((hkReferencedObject *)&finalConn->base);

            if (edgeCapFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, edges, 8 * edgeCapFlags, HK_MEMORY_CLASS_ARRAY);
            hkPointerMapBase_destruct(&edgeMap);
            if (newFacePlanes.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, newFacePlanes.m_data, 16 * newFacePlanes.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
            hkThreadMemory_deallocateStack(mem, cutVertsBase);
            hkThreadMemory_deallocateStack(mem, vinfo);
            hkThreadMemory_deallocateStack(mem, origVertsBase);
            return result;
        }
        else
        {
            hkReferencedObject_removeReference((hkReferencedObject *)&finalConn->base);
            if (edgeCapFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, edges, 8 * edgeCapFlags, HK_MEMORY_CLASS_ARRAY);
            hkPointerMapBase_destruct(&edgeMap);
            if (newFacePlanes.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, newFacePlanes.m_data, 16 * newFacePlanes.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
            hkThreadMemory_deallocateStack(mem, cutVertsBase);
            hkThreadMemory_deallocateStack(mem, vinfo);
            hkThreadMemory_deallocateStack(mem, origVertsBase);
            return nullptr;
        }
    }
}
