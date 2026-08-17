/* ?setSkinningValues@hkSkinningUtil@@SA?AW4hkResult@@ABV?$hkArray@UEntry@hkSkinningUtil@@@@PAVhkMeshVertexBuffer@@M@Z @0x839E6DE0 */
#include "../headers/havok/hkSkinningUtil_Entry.h"
#include "../headers/havok/hkMeshSection_boundary.h"
#include "../headers/havok/hkVertexFormat.h"
#include "../headers/havok/hkThreadMemory.h"

/* boundary — vertex-buffer virtuals and the format-conversion writer (level-2). */
extern int  hkMeshVertexBuffer_getNumVertices(hkMeshVertexBuffer *self);       /* virtual getNumVertices */
extern void hkMeshVertexBuffer_unlock(hkMeshVertexBuffer *self, const hkMeshVertexBuffer_LockedVertices *locked);
extern void hkMeshVertexBufferUtil_convert(const hkMeshVertexBuffer_LockedVertices_Buffer *src, const hkMeshVertexBuffer_LockedVertices_Buffer *dst, int numVertices); /* hkMeshVertexBufferUtil::convert */

#define HK_SKIN_MIN_WEIGHT 0.001f

/* hkSkinningUtil::setSkinningValues — turn a per-vertex list of candidate bone
   bindings into normalized blend indices + weights written into the mesh's
   vertex buffer. For each vertex the routine keeps every valid bone slot, giving
   bones nearer than maxDistance a weight of (1 - d^2/maxDistance^2) (bones at or
   beyond maxDistance, and any degenerate case, fall back to a tiny floor weight),
   then normalizes the per-vertex weights to sum to 1. The computed index/weight
   scratch arrays are converted into the vertex buffer's BLEND_MATRIX_INDEX and
   BLEND_WEIGHTS elements. Returns 0 on success, 1 if the buffer lacks the blend
   elements or cannot be locked.
   DEVIATION: the two scratch arrays are taken from the Havok per-thread stack
   (the original inlines the bump allocator); the fsel-based weight floor and the
   4-wide unrolled normalization are reconstructed as straight scalar loops; the
   source Buffer descriptors and partial-lock inputs are built with the DB-typed
   structures rather than the original's raw field stores. */
int hkSkinningUtil_setSkinningValues(const hkArray<hkSkinningUtil_Entry> *entries,
                                     hkMeshVertexBuffer *vertexBuffer, float maxDistance)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    hkVertexFormat format;
    int numVertices;
    int matrixIndexElem;
    int weightElem;
    int bonesPerVertex;
    int total;
    float *weights;
    unsigned char *indices;
    float maxDistSq;
    float invMaxDistSq;
    int base;
    int v;

    numVertices = hkMeshVertexBuffer_getNumVertices(vertexBuffer);
    hkVertexFormat_construct(&format);
    hkMeshVertexBuffer_getVertexFormat(vertexBuffer, &format);

    matrixIndexElem = hkVertexFormat_findElementIndex(&format, USAGE_BLEND_MATRIX_INDEX, 0);
    weightElem = hkVertexFormat_findElementIndex(&format, USAGE_BLEND_WEIGHTS, 0);
    if (weightElem < 0)
        weightElem = hkVertexFormat_findElementIndex(&format, USAGE_BLEND_WEIGHTS_LAST_IMPLIED, 0);
    if (matrixIndexElem < 0 || weightElem < 0)
        return 1;

    bonesPerVertex = format.m_elements[matrixIndexElem].m_numValues;
    total = bonesPerVertex * numVertices;

    weights = (float *)hkThreadMemory_allocateStack(mem, (total + 4) * 4);
    indices = (unsigned char *)hkThreadMemory_allocateStack(mem, (total + 16) & ~15);

    maxDistSq = maxDistance * maxDistance;
    invMaxDistSq = (maxDistSq <= 0.0f) ? 0.0f : (1.0f / maxDistSq);

    base = 0;
    for (v = 0; v < numVertices; ++v)
    {
        const hkSkinningUtil_Entry *entry = &entries->m_data[v * bonesPerVertex];
        float sum = 0.0f;
        int contributing = 0;
        int k;

        for (k = 0; k < bonesPerVertex; ++k)
        {
            if (entry[k].m_index < 0)
            {
                indices[base + k] = 0;
                weights[base + k] = 0.0f;
            }
            else
            {
                float w = (entry[k].m_distanceSquared >= maxDistSq)
                          ? 0.0f
                          : -(entry[k].m_distanceSquared * invMaxDistSq - 1.0f);
                if (HK_SKIN_MIN_WEIGHT - w >= 0.0f)
                    w = HK_SKIN_MIN_WEIGHT;
                indices[base + k] = (unsigned char)entry[k].m_index;
                weights[base + k] = w;
                sum += w;
                ++contributing;
            }
        }

        if (contributing <= 0)
        {
            weights[base] = 1.0f;
        }
        else
        {
            float invSum = 1.0f / sum;
            for (k = 0; k < contributing; ++k)
                weights[base + k] *= invSum;
        }
        base += bonesPerVertex;
    }

    /* Write the scratch arrays back into the vertex buffer's blend elements. */
    {
        hkMeshVertexBuffer_LockInput lockInput;
        hkMeshVertexBuffer_PartialLockInput partial;
        hkMeshVertexBuffer_LockedVertices locked;
        Buffer indexSrc;
        Buffer weightSrc;

        lockInput.m_startVertex = 0;
        lockInput.m_numVertices = -1;
        lockInput.m_noWait.m_bool = 0;          /* hkBool wraps a char (m_bool) */
        lockInput.m_contiguousAccess.m_bool = 0;
        lockInput.m_lockFlags = 0;

        partial.m_numLockFlags = 2;
        partial.m_elementIndices[0] = matrixIndexElem;
        partial.m_elementIndices[1] = weightElem;
        partial.m_lockFlags[0] = 6;
        partial.m_lockFlags[1] = 6;

        weightSrc.m_start = weights;
        weightSrc.m_stride = 4 * bonesPerVertex;
        weightSrc.m_element.m_usage.m_storage = USAGE_BLEND_WEIGHTS;
        weightSrc.m_element.m_dataType.m_storage = 10;
        weightSrc.m_element.m_numValues = (unsigned char)bonesPerVertex;
        weightSrc.m_element.m_subUsage = 0;
        weightSrc.m_element.m_flags.m_storage = 0;

        indexSrc.m_start = indices;
        indexSrc.m_stride = bonesPerVertex;
        indexSrc.m_element.m_usage.m_storage = USAGE_BLEND_WEIGHTS;
        indexSrc.m_element.m_dataType.m_storage = 2;
        indexSrc.m_element.m_numValues = (unsigned char)bonesPerVertex;
        indexSrc.m_element.m_subUsage = 0;
        indexSrc.m_element.m_flags.m_storage = 0;

        if (hkMeshVertexBuffer_partialLock(vertexBuffer, &lockInput, &partial, &locked) != RESULT_SUCCESS)
        {
            hkThreadMemory_deallocateStack(mem, indices);
            hkThreadMemory_deallocateStack(mem, weights);
            return 1;
        }

        hkMeshVertexBufferUtil_convert(&indexSrc, &locked.m_buffers[0], numVertices);
        hkMeshVertexBufferUtil_convert(&weightSrc, &locked.m_buffers[1], numVertices);
        hkMeshVertexBuffer_unlock(vertexBuffer, &locked);
    }

    hkThreadMemory_deallocateStack(mem, indices);
    hkThreadMemory_deallocateStack(mem, weights);
    return 0;
}
