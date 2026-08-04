#include "../headers/havok/hkError.h"
#include "../headers/havok/hkMeshVertexBuffer_Buffer.h"

/* hkMeshVertexBufferUtil::reportUnconvertibleFormat — the diagnostic tail of
   hkMeshVertexBufferUtil::convert (inlined @0x839ED7F0, block 0x839ED9C8-0x839EDA44).
   Builds "Cannot convert vertex format from <srcType> to <dstType>" into a 512-byte
   hkErrStream and routes it to the global hkError sink as a WARNING.

   DEVIATION: reconstructed from the inlined call site; convert.cpp factors this
   branch out to keep its dispatch legible. The stream reads src->m_element.
   m_dataType.m_storage and dst->m_element.m_dataType.m_storage (each at offset 8,
   lbz 8(rN) @0x839ED9E4/0x839ED9E8). The virtual hkError::message return value is
   NOT checked here (no branch after bctrl @0x839EDA3C), so there is no trap — unlike
   the ERROR-severity hkReferenceCountError path. */
void hkMeshVertexBufferUtil_reportUnconvertibleFormat(
    const hkMeshVertexBuffer_LockedVertices_Buffer *src,
    const hkMeshVertexBuffer_LockedVertices_Buffer *dst)
{
    hkErrStream stream;
    char buffer[512];
    hkOstream *os;
    int srcType = src->m_element.m_dataType.m_storage;
    int dstType = dst->m_element.m_dataType.m_storage;

    hkErrStream_construct(&stream, buffer, 512);
    os = hkOstream_insertString((hkOstream *)&stream, "Cannot convert vertex format from ");
    os = hkOstream_insertInt(os, srcType);
    os = hkOstream_insertString(os, " to ");
    hkOstream_insertInt(os, dstType);
    /* r4=1 => MESSAGE_WARNING; id 0xABBA4523 @0x839EDA14/0x839EDA28; line 0x404 @0x839EDA18 */
    hkError_message(hkError_s_instance, MESSAGE_WARNING, (int)0xABBA4523, buffer,
                    ".\\Mesh\\Utils\\VertexBufferUtil\\hkMeshVertexBufferUtil.cpp", 1028);
    hkOstream_destruct((hkOstream *)&stream);
}
