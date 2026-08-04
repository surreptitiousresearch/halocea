/* boundary — Havok's byte copy (sibling TU hkString_memCpy). */
extern void hkString_memCpy(void *dst, const void *src, int numBytes);

/* hkMeshVertexBufferUtil::stridedCopy — copy a fixed byte span (bytesPerVertex)
   from one strided vertex stream to another, numVertices times. When the two
   strides equal each other and the element size, the whole region is one
   contiguous block and is copied in a single memcpy; otherwise each vertex is
   copied individually at its src/dst stride.

   DEVIATION: the DB body specialises the per-vertex copy into dword-count cases
   (1/2/3-dword unrolled loops) when bytesPerVertex is a small multiple of 4;
   those are numerically equivalent to the per-vertex hkString::memCpy used here
   and are collapsed for legibility. */
void hkMeshVertexBufferUtil_stridedCopy(const void *srcIn, int srcStride,
                                        void *dstIn, int dstStride,
                                        int elementSize, int numVertices)
{
    const char *src = (const char *)srcIn;
    char *dst = (char *)dstIn;
    int i;

    if (elementSize == 0)
        return;

    if (srcStride == dstStride && srcStride == elementSize)
    {
        hkString_memCpy(dst, src, elementSize * numVertices);
        return;
    }

    for (i = 0; i < numVertices; ++i)
    {
        hkString_memCpy(dst, src, elementSize);
        src += srcStride;
        dst += dstStride;
    }
}
