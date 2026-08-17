/* ?checkConsistency@hkdGeometry@@QBA?AW4hkResult@@XZ @0x83852338 */
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdGeometry_Face.h"
#include "../headers/havok/hkdGeometry_ObjectIdentifier.h"
#include "../headers/havok/hkdGeometry_HalfEdge.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkString.h"
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkThreadMemory.h"

/* Boundary callees (2-level descent). The assertion/report path (hkString::printf
   into a scratch hkString, hkErrStream to the global hkError sink, __trap on a
   fatal message) and the geometric helpers are all externs here. */
extern void hkString_printf(hkString *self, const char *fmt, ...);                                     /* hkString::printf */
extern void hkdGeometry_calculateHalfEdges(hkdGeometry *self, hkArray<HalfEdge> *halfEdgesOut);        /* hkdGeometry::calculateHalfEdges */
extern int  sortEdges(const HalfEdge *a, const HalfEdge *b);                                           /* file-scope comparator */
/* DEVIATION: template instantiation
   hkAlgorithm::quickSortRecursive<hkdGeometry::HalfEdge, bool(*)(...)>. */
extern void hkAlgorithm_quickSortRecursive_HalfEdge(HalfEdge *base, int firstIndex, int lastIndex,
                                                    int (*less)(const HalfEdge *, const HalfEdge *));
extern hkBool *hkMatrix4_isOk(hkMatrix4 *scratch, const hkMatrix4 *m);                                 /* hkMatrix4::isOk */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);                                        /* hkArrayUtil::_reserveMore */

#define GEOMETRY_CPP "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\BaseTypes\\Geometry\\hkdGeometry.cpp"

/* Emit one error line built from the scratch hkString into the global error sink.
   Returns nonzero when the sink requests a trap (fatal). */
static int report(hkThreadMemory *mem, const hkString *msg, int id, int line)
{
    char buffer[512];
    hkErrStream es;
    int fatal;
    (void)mem;
    hkErrStream_construct(&es, buffer, 512);
    hkOstream_insertString(&es.base, msg->m_string.m_data);
    fatal = hkError_message(hkError_s_instance, MESSAGE_ERROR, id, buffer, GEOMETRY_CPP, line);
    if (fatal)
        __trap();
    hkOstream_destruct(&es.base);
    return fatal;
}

/* hkdGeometry::checkConsistency — validate a fracture geometry before it is used:
   table sizes fit in 16 bits, the half-edge graph is manifold (every edge shared
   by an even number of faces), every vertex is referenced, every face names a
   valid source object and owns at least one triangle, every triangle is indexed
   by exactly one face, and every source-object transform is well formed.
   Returns 1 on the first inconsistency found, 0 if the geometry is consistent. */
int hkdGeometry_checkConsistency(hkdGeometry *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    hkString msg;
    int numVertices = self->m_vertices.m_size;
    int *vertexUseCount;
    hkArray<HalfEdge> halfEdges;
    hkArray<HalfEdge> edges;
    int numObjects;
    int *triangleUsed;
    int numTriangleWords;
    int i;

    /* Default-constructed scratch hkString (one NUL byte). */
    msg.m_string.m_data = (char *)hkThreadMemory_allocateChunk(mem, 1, HK_MEMORY_CLASS_ARRAY);
    msg.m_string.m_size = 1;
    msg.m_string.m_capacityAndFlags = 1;
    msg.m_string.m_data[0] = 0;

    if (numVertices >= 0x10000)
    {
        hkString_printf(&msg, "Too many vertices (%i) - allowed (%i)", numVertices, 0x10000);
        report(mem, &msg, -1413873515, 644);
        goto fail;
    }
    if (self->m_faces.m_size >= 0x10000)
    {
        hkString_printf(&msg, "Too many faces (%i) - allowed (%i)", self->m_faces.m_size, 0x10000);
        report(mem, &msg, -1413873514, 650);
        goto fail;
    }
    if (self->m_triangles.m_size >= 0x10000)
    {
        hkString_printf(&msg, "Too many triangles (%i) - allowed (%i)", self->m_triangles.m_size, 0x10000);
        report(mem, &msg, -1413873513, 656);
        goto fail;
    }

    /* --- Half-edge manifold check ------------------------------------------ */
    vertexUseCount = (int *)hkThreadMemory_allocateChunk(mem, 4 * numVertices, HK_MEMORY_CLASS_ARRAY);
    for (i = 0; i < numVertices; ++i)
        vertexUseCount[i] = 0;

    edges.m_data = 0;
    edges.m_size = 0;
    edges.m_capacityAndFlags = 0x80000000;

    halfEdges.m_data = 0;
    halfEdges.m_size = 0;
    halfEdges.m_capacityAndFlags = 0x80000000;
    hkdGeometry_calculateHalfEdges(self, &halfEdges);

    for (i = 0; i < halfEdges.m_size; ++i)
    {
        HalfEdge he = halfEdges.m_data[i];
        HalfEdge canon;
        int store = 1;

        if (he.m_neighbouringFace == 0xFFFF)
        {
            /* Boundary half-edge: order the endpoints; drop degenerate ones. */
            if (he.m_a == he.m_b)
                store = 0;
            else if (he.m_a < he.m_b)
            {
                canon.m_a = he.m_a; canon.m_b = he.m_b;
                canon.m_face = he.m_face; canon.m_neighbouringFace = he.m_neighbouringFace;
            }
            else
            {
                canon.m_a = he.m_b; canon.m_b = he.m_a;
                canon.m_face = he.m_face; canon.m_neighbouringFace = he.m_neighbouringFace;
            }
        }
        else
        {
            /* Interior half-edge: canonicalise so the lower vertex leads and the
               owning/neighbour faces follow suit. */
            int swap = (he.m_a == he.m_b) ? (he.m_face >= he.m_neighbouringFace)
                                          : (he.m_a >= he.m_b);
            if (swap)
            {
                canon.m_a = he.m_b; canon.m_b = he.m_a;
                canon.m_face = he.m_neighbouringFace; canon.m_neighbouringFace = he.m_face;
            }
            else
            {
                canon.m_a = he.m_a; canon.m_b = he.m_b;
                canon.m_face = he.m_face; canon.m_neighbouringFace = he.m_neighbouringFace;
            }
        }

        if (store)
        {
            if (edges.m_size == (edges.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&edges.m_data, 8);
            edges.m_data[edges.m_size++] = canon;
            ++vertexUseCount[he.m_a];
            ++vertexUseCount[he.m_b];
        }
    }

    /* Append the end-of-list sentinel, sort, then pop it back off. */
    if (edges.m_size == (edges.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&edges.m_data, 8);
    {
        HalfEdge sentinel;
        sentinel.m_a = 0xFFFF; sentinel.m_b = 0xFFFF;
        sentinel.m_face = 0xFFFF; sentinel.m_neighbouringFace = 0xFFFF;
        edges.m_data[edges.m_size++] = sentinel;
    }
    --edges.m_size;
    if (edges.m_size > 1)
        hkAlgorithm_quickSortRecursive_HalfEdge(edges.m_data, 0, edges.m_size - 1, sortEdges);

    /* Walk runs of identical edges; a manifold surface shares each edge an even
       number of times. */
    {
        HalfEdge *cur = edges.m_data;
        HalfEdge *end = &edges.m_data[edges.m_size];
        while (cur < end)
        {
            HalfEdge *run = cur + 1;
            while (run->m_a == cur->m_a && run->m_b == cur->m_b
                   && run->m_face == cur->m_face && run->m_neighbouringFace == cur->m_neighbouringFace)
                ++run;
            if (((run - cur) & 1) != 0)
            {
                hkString_printf(&msg, "Found an uneven number (%i) of edges between vertex index %i and %i",
                                (int)(run - cur), cur->m_a, cur->m_b);
                report(mem, &msg, -1413873512, 757);
                if (numVertices >= 0)
                    hkThreadMemory_deallocateChunk(mem, vertexUseCount, 4 * numVertices, HK_MEMORY_CLASS_ARRAY);
                if ((halfEdges.m_capacityAndFlags & 0x80000000) == 0)
                    hkThreadMemory_deallocateChunk(mem, halfEdges.m_data, 8 * halfEdges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
                if (edges.m_capacityAndFlags >= 0)
                    hkThreadMemory_deallocateChunk(mem, edges.m_data, 8 * (edges.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
                goto fail;
            }
            cur = run;
            if (run >= end)
                break;
        }
    }

    /* --- Every vertex referenced ------------------------------------------- */
    for (i = 0; i < numVertices; ++i)
    {
        if ((vertexUseCount[i] & 1) == 0)
        {
            const float *pos = self->m_vertices.m_data[i].m_quad.___u0.v;
            /* DEVIATION: the unused-vertex format string is a data global
               (off_82130000) not recovered here. */
            hkString_printf(&msg, "An unreferenced vertex (%f, %f, %f) was found", pos[0], pos[1], pos[2]);
            report(mem, &msg, -1413873511, 772);
            hkThreadMemory_deallocateChunk(mem, vertexUseCount, 4 * numVertices, HK_MEMORY_CLASS_ARRAY);
            if ((halfEdges.m_capacityAndFlags & 0x80000000) == 0)
                hkThreadMemory_deallocateChunk(mem, halfEdges.m_data, 8 * halfEdges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
            if (edges.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, edges.m_data, 8 * (edges.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
            goto fail;
        }
    }

    if (numVertices >= 0)
        hkThreadMemory_deallocateChunk(mem, vertexUseCount, 4 * numVertices, HK_MEMORY_CLASS_ARRAY);
    if ((halfEdges.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(mem, halfEdges.m_data, 8 * halfEdges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (edges.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, edges.m_data, 8 * (edges.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    /* --- Face / triangle indexing ------------------------------------------ */
    numTriangleWords = (self->m_triangles.m_size + 31) >> 5;
    triangleUsed = (int *)hkThreadMemory_allocateChunk(mem, 4 * numTriangleWords, HK_MEMORY_CLASS_ARRAY);
    for (i = 0; i < numTriangleWords; ++i)
        triangleUsed[i] = 0;

    numObjects = self->m_parent->m_objectIds.m_size;
    for (i = 0; i < self->m_faces.m_size; ++i)
    {
        Face *face = &self->m_faces.m_data[i];
        int t;

        if (face->m_numTriangles == 0)
        {
            hkString_printf(&msg, "Found no triangles");
            report(mem, &msg, -1413873510, 795);
            if (numTriangleWords >= 0)
                hkThreadMemory_deallocateChunk(mem, triangleUsed, 4 * numTriangleWords, HK_MEMORY_CLASS_ARRAY);
            goto fail;
        }
        if (face->m_objectIndex >= numObjects)
        {
            hkString_printf(&msg, "An invalid object is referenced");
            report(mem, &msg, -1413873508, 802);
            if (numTriangleWords >= 0)
                hkThreadMemory_deallocateChunk(mem, triangleUsed, 4 * numTriangleWords, HK_MEMORY_CLASS_ARRAY);
            goto fail;
        }
        for (t = face->m_startTriangleIndex; t < face->m_startTriangleIndex + face->m_numTriangles; ++t)
            triangleUsed[t >> 5] |= 1 << (t & 0x1F);
    }

    for (i = 0; i < self->m_triangles.m_size; ++i)
    {
        if ((triangleUsed[i >> 5] & (1 << (i & 0x1F))) == 0)
        {
            hkString_printf(&msg, "An unindexed triangle is found");
            report(mem, &msg, -1413873507, 823);
            if (numTriangleWords >= 0)
                hkThreadMemory_deallocateChunk(mem, triangleUsed, 4 * numTriangleWords, HK_MEMORY_CLASS_ARRAY);
            goto fail;
        }
    }

    if (numTriangleWords >= 0)
        hkThreadMemory_deallocateChunk(mem, triangleUsed, 4 * numTriangleWords, HK_MEMORY_CLASS_ARRAY);

    /* --- Source-object transforms ------------------------------------------ */
    for (i = 0; i < self->m_parent->m_objectIds.m_size; ++i)
    {
        hkMatrix4 scratch;
        if (!hkMatrix4_isOk(&scratch, &self->m_parent->m_objectIds.m_data[i].m_transform)->m_bool)
        {
            hkString_printf(&msg, "An invalid transform is used");
            report(mem, &msg, -1413873506, 836);
            goto fail;
        }
    }

    if (msg.m_string.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, msg.m_string.m_data,
                                       msg.m_string.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
    return 0;

fail:
    if (msg.m_string.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, msg.m_string.m_data,
                                       msg.m_string.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
    return 1;
}
