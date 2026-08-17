/* hkMeshVertexBufferUtil_convertUint8ToUint8Dword @0x839ECF30 */
#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"

/* hkMeshVertexBufferUtil::convertUint8ToUint8Dword — pack a UINT8 element (one
   byte per component) into a UINT8_DWORD element (four uint8 components per
   32-bit word). Runs only when the source is data type 2 (UINT8), the
   destination is data type 7 (UINT8_DWORD), and the component counts match.

   Four source bytes (b0,b1,b2,b3) are packed little-endian into one destination
   word: b0 | b1<<8 | b2<<16 | b3<<24. Components are consumed in groups of four
   with a 1/2/3 remainder tail (partial word), matching the DB body. */
void hkMeshVertexBufferUtil_convertUint8ToUint8Dword(
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
        || srcBuffer->m_element.m_dataType.m_storage != 2
        || dstBuffer->m_element.m_dataType.m_storage != 7)
        return;

    srcStride = srcBuffer->m_stride;
    dstStride = dstBuffer->m_stride;
    srcRow = (const char *)srcBuffer->m_start;
    dstRow = (char *)dstBuffer->m_start;

    for (v = 0; v < numVertices; ++v)
    {
        const unsigned char *sp = (const unsigned char *)srcRow;
        unsigned int *dp = (unsigned int *)dstRow;
        int remaining = numValues;

        while (remaining >= 4)
        {
            *dp++ = (unsigned int)sp[0]
                  | ((unsigned int)sp[1] << 8)
                  | ((unsigned int)sp[2] << 16)
                  | ((unsigned int)sp[3] << 24);
            sp += 4;
            remaining -= 4;
        }
        if (remaining == 1)
            *dp = (unsigned int)sp[0];
        else if (remaining == 2)
            *dp = (unsigned int)sp[0] | ((unsigned int)sp[1] << 8);
        else if (remaining == 3)
            *dp = (unsigned int)sp[0] | ((unsigned int)sp[1] << 8) | ((unsigned int)sp[2] << 16);

        srcRow += srcStride;
        dstRow += dstStride;
    }
}
