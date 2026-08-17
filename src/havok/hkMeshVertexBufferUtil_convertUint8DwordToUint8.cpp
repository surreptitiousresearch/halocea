/* hkMeshVertexBufferUtil_convertUint8DwordToUint8 @0x839ECD30 */
#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"

/* hkMeshVertexBufferUtil::convertUint8DwordToUint8 — unpack a UINT8_DWORD element
   (four uint8 components packed into one 32-bit word) into a UINT8 element (one
   byte per component). Runs only when the source is data type 7 (UINT8_DWORD),
   the destination is data type 2 (UINT8), and the two component counts match.

   Each source word yields up to four destination bytes; per the DB body the byte
   order written is (byte0, byte2, byte1, byte3). Components are consumed in
   groups of four (one source word each) with a 1/2/3 remainder tail. */
void hkMeshVertexBufferUtil_convertUint8DwordToUint8(
        const hkMeshVertexBuffer_LockedVertices_Buffer *srcBuffer,
        const hkMeshVertexBuffer_LockedVertices_Buffer *dstBuffer,
        int numVertices)
{
    int numValues = srcBuffer->m_element.m_numValues;
    int srcStride, dstStride;
    const char *srcRow;
    char *dstRow;
    int v;

    if (numValues != dstBuffer->m_element.m_numValues
        || srcBuffer->m_element.m_dataType.m_storage != 7
        || dstBuffer->m_element.m_dataType.m_storage != 2)
        return;

    srcStride = srcBuffer->m_stride;
    dstStride = dstBuffer->m_stride;
    srcRow = (const char *)srcBuffer->m_start;
    dstRow = (char *)dstBuffer->m_start;

    for (v = 0; v < numVertices; ++v)
    {
        const unsigned int *sp = (const unsigned int *)srcRow;
        unsigned char *dp = (unsigned char *)dstRow;
        int remaining = numValues;

        while (remaining >= 4)
        {
            unsigned int w = *sp++;
            dp[0] = (unsigned char)(w);
            dp[1] = (unsigned char)(w >> 16);
            dp[2] = (unsigned char)(w >> 8);
            dp[3] = (unsigned char)(w >> 24);
            dp += 4;
            remaining -= 4;
        }
        if (remaining >= 1)
        {
            unsigned int w = *sp;
            dp[0] = (unsigned char)(w);
            if (remaining >= 2)
                dp[1] = (unsigned char)(w >> 16);
            if (remaining >= 3)
                dp[2] = (unsigned char)(w >> 8);
        }

        srcRow += srcStride;
        dstRow += dstStride;
    }
}
