#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"

/* hkMeshVertexBufferUtil::convertFloat32ToUint8 @ 0x839ED4C8
   Convert a float32 vertex element stream to unsigned-byte (value * 255, C-style
   truncation toward zero), per vertex, honouring each buffer's stride.

   Three component-count/usage cases (usage 7 = BLEND_WEIGHTS, 8 =
   BLEND_WEIGHTS_LAST_IMPLIED):
     - equal component counts: straight per-lane conversion;
     - src BLEND_WEIGHTS -> dst LAST_IMPLIED with dst one wider: convert dst-count
       lanes straight;
     - dst LAST_IMPLIED (or usage 7) with dst one narrower: convert the explicit
       lanes and store the implied last lane as 255 - sum(others).

   DEVIATION: the binary unrolls the per-lane loops (x4 / x2) with VMX float->int;
   reconstructed as scalar loops with identical results. */
void hkMeshVertexBufferUtil_convertFloat32ToUint8(const hkMeshVertexBuffer_LockedVertices_Buffer *srcBuffer,
                                                  const hkMeshVertexBuffer_LockedVertices_Buffer *dstBuffer,
                                                  int numVertices)
{
    int srcCount = srcBuffer->m_element.m_numValues;
    int dstCount = dstBuffer->m_element.m_numValues;
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;
    const float *src = (const float *)srcBuffer->m_start;
    unsigned char *dst = (unsigned char *)dstBuffer->m_start;
    int srcUsage = srcBuffer->m_element.m_usage.m_storage;
    int dstUsage = dstBuffer->m_element.m_usage.m_storage;
    int v, j;

    if (srcCount == dstCount)
    {
        for (v = 0; v < numVertices; ++v)
        {
            for (j = 0; j < srcCount; ++j)
                dst[j] = (unsigned char)(int)(src[j] * 255.0f);
            src = (const float *)((const char *)src + srcStride);
            dst += dstStride;
        }
    }
    else if (srcUsage == 7 && dstUsage == 8 && srcCount == dstCount - 1)
    {
        for (v = 0; v < numVertices; ++v)
        {
            for (j = 0; j < dstCount; ++j)
                dst[j] = (unsigned char)(int)(src[j] * 255.0f);
            src = (const float *)((const char *)src + srcStride);
            dst += dstStride;
        }
    }
    else if ((srcUsage == 8 || dstUsage == 7) && dstCount == srcCount - 1)
    {
        for (v = 0; v < numVertices; ++v)
        {
            unsigned char sum = 0;
            for (j = 0; j < dstCount; ++j)
            {
                unsigned char b = (unsigned char)(int)(src[j] * 255.0f);
                dst[j] = b;
                sum = (unsigned char)(sum + b);
            }
            dst[dstCount] = (unsigned char)(255 - sum);
            src = (const float *)((const char *)src + srcStride);
            dst += dstStride;
        }
    }
}
