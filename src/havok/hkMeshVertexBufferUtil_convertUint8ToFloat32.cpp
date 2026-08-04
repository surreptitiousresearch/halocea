#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"

/* hkMeshVertexBufferUtil::convertUint8ToFloat32 @ 0x839ED158
   Convert an unsigned-byte vertex element stream to float32 (byte * 1/255), per
   vertex, honouring each buffer's stride. Inverse of convertFloat32ToUint8, with
   the same three component-count/usage cases (usage 7 = BLEND_WEIGHTS, 8 =
   BLEND_WEIGHTS_LAST_IMPLIED):
     - equal component counts: straight per-lane conversion;
     - src BLEND_WEIGHTS -> dst LAST_IMPLIED with dst one wider: convert dst-count
       lanes straight;
     - dst one narrower: convert the explicit lanes and synthesise the implied
       last float from (255 - sum(explicit source bytes)) / 255.

   DEVIATION: the binary unrolls the per-lane loops (x4 / x2) and packs both
   buffer pointers into one 64-bit register; reconstructed as scalar loops. */
void hkMeshVertexBufferUtil_convertUint8ToFloat32(const hkMeshVertexBuffer_LockedVertices_Buffer *srcBuffer,
                                                  const hkMeshVertexBuffer_LockedVertices_Buffer *dstBuffer,
                                                  int numVertices)
{
    const float INV_255 = 1.0f / 255.0f;
    int srcCount = srcBuffer->m_element.m_numValues;
    int dstCount = dstBuffer->m_element.m_numValues;
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;
    const unsigned char *src = (const unsigned char *)srcBuffer->m_start;
    float *dst = (float *)dstBuffer->m_start;
    int srcUsage = srcBuffer->m_element.m_usage.m_storage;
    int dstUsage = dstBuffer->m_element.m_usage.m_storage;
    int v, j;

    if (srcCount == dstCount)
    {
        for (v = 0; v < numVertices; ++v)
        {
            for (j = 0; j < srcCount; ++j)
                dst[j] = (float)src[j] * INV_255;
            src += srcStride;
            dst = (float *)((char *)dst + dstStride);
        }
    }
    else if (srcUsage == 7 && dstUsage == 8 && srcCount == dstCount - 1)
    {
        for (v = 0; v < numVertices; ++v)
        {
            for (j = 0; j < dstCount; ++j)
                dst[j] = (float)src[j] * INV_255;
            src += srcStride;
            dst = (float *)((char *)dst + dstStride);
        }
    }
    else if ((srcUsage == 8 || dstUsage == 7) && dstCount == srcCount - 1)
    {
        for (v = 0; v < numVertices; ++v)
        {
            unsigned char sum = 0;
            for (j = 0; j < dstCount; ++j)
            {
                unsigned char b = src[j];
                dst[j] = (float)b * INV_255;
                sum = (unsigned char)(sum + b);
            }
            dst[dstCount] = (float)(unsigned char)(255 - sum) * INV_255;
            src += srcStride;
            dst = (float *)((char *)dst + dstStride);
        }
    }
}
