#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"
#include "../headers/havok/hkVertexFormat.h"

/* Per-data-type element size table (hkVertexFormat::s_dataTypeToSize), indexed by
   the DataType storage value. Boundary — defined in the Havok vertex-format TU. */
extern const int hkVertexFormat_s_dataTypeToSize[];

/* Copy a fixed byte span per vertex from one strided stream to another. */
extern void hkMeshVertexBufferUtil_stridedCopy(const void *src, int srcStride,
                                               void *dst, int dstStride,
                                               int bytesPerVertex, int numVertices);

/* Per-format lane converters (boundary — sibling TUs in hkMeshVertexBufferUtil). */
extern void hkMeshVertexBufferUtil_convertUint8DwordToUint8(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst, int numVertices);
extern void hkMeshVertexBufferUtil_convertUint8ToUint8Dword(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst, int numVertices);
extern void hkMeshVertexBufferUtil_convertUint8ToFloat32(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst, int numVertices);
extern void hkMeshVertexBufferUtil_convertFloat32ToUint8(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst, int numVertices);

/* DEVIATION: the original's else-branch inlines an hkErrStream/hkOstream chain
   and hkError::message warning ("Cannot convert vertex format from X to Y").
   That diagnostic scaffolding is factored into this boundary helper to keep the
   dispatch logic legible; behaviour on the unsupported path is unchanged. */
extern void hkMeshVertexBufferUtil_reportUnconvertibleFormat(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst);

/* hkMeshVertexBufferUtil::convert — copy/convert numVertices worth of a single
   vertex element from srcBuffer to dstBuffer. When the data types and component
   counts match it is a straight strided copy; when only the counts differ but
   the source has at least as many components it copies the destination width;
   otherwise it dispatches to a per-type lane converter. */
void hkMeshVertexBufferUtil_convert(const hkMeshVertexBuffer_LockedVertices_Buffer *srcBuffer,
                                    const hkMeshVertexBuffer_LockedVertices_Buffer *dstBuffer,
                                    int numVertices)
{
    int srcType = srcBuffer->m_element.m_dataType.m_storage;
    int dstType = dstBuffer->m_element.m_dataType.m_storage;

    if (srcType == dstType)
    {
        int srcVals = srcBuffer->m_element.m_numValues;
        if (srcVals == dstBuffer->m_element.m_numValues)
        {
            hkMeshVertexBufferUtil_stridedCopy(
                srcBuffer->m_start, srcBuffer->m_stride,
                dstBuffer->m_start, dstBuffer->m_stride,
                (hkVertexFormat_s_dataTypeToSize[srcType] * srcVals + 3) & ~3,
                numVertices);
            return;
        }
        {
            int dstVals = dstBuffer->m_element.m_numValues;
            if (srcVals >= dstVals)
            {
                hkMeshVertexBufferUtil_stridedCopy(
                    srcBuffer->m_start, srcBuffer->m_stride,
                    dstBuffer->m_start, dstBuffer->m_stride,
                    (hkVertexFormat_s_dataTypeToSize[dstType] * dstVals + 3) & ~3,
                    numVertices);
                return;
            }
        }
    }

    switch (srcType)
    {
    case 7:
        if (dstType == 2)
            hkMeshVertexBufferUtil_convertUint8DwordToUint8(srcBuffer, dstBuffer, numVertices);
        break;
    case 2:
        if (dstType == 7)
            hkMeshVertexBufferUtil_convertUint8ToUint8Dword(srcBuffer, dstBuffer, numVertices);
        else if (dstType == 10)
            hkMeshVertexBufferUtil_convertUint8ToFloat32(srcBuffer, dstBuffer, numVertices);
        break;
    case 10:
        if (dstType == 2)
            hkMeshVertexBufferUtil_convertFloat32ToUint8(srcBuffer, dstBuffer, numVertices);
        break;
    default:
        if (srcType == 11 && dstType == 10
            && (unsigned int)dstBuffer->m_element.m_numValues <= 4
            && srcBuffer->m_element.m_numValues == 1)
        {
            hkMeshVertexBufferUtil_stridedCopy(
                srcBuffer->m_start, srcBuffer->m_stride,
                dstBuffer->m_start, dstBuffer->m_stride,
                4 * dstBuffer->m_element.m_numValues, numVertices);
        }
        else if ((srcType == 6 && srcBuffer->m_element.m_numValues == 1 && dstType == 8)
              || (srcType == 8 && srcBuffer->m_element.m_numValues == 1 && dstType == 6))
        {
            hkMeshVertexBufferUtil_stridedCopy(
                srcBuffer->m_start, srcBuffer->m_stride,
                dstBuffer->m_start, dstBuffer->m_stride,
                4, numVertices);
        }
        else
        {
            hkMeshVertexBufferUtil_reportUnconvertibleFormat(srcBuffer, dstBuffer);
        }
        break;
    }
}
