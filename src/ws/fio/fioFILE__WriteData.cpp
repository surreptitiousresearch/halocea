/* CAVEAT: byte-reorder-on-write scratch buffer. When the stream has FIO_OPEN_BYTE_REORDER
 * set and the element size (`reorderGain`) is > 1 bytes, each element is endian-swapped
 * into a shared scratch dsVECTOR<unsigned char,8> before being handed to DoWriteData.
 * That scratch vector pointer lives at offset 16 of a process-global fio singleton loaded
 * from the small-data area (0(r13)); the singleton's full layout is UNRESOLVED, so the
 * +16 slot is reached by explicit offset. The lazy allocation reproduces the inlined
 * dsVECTOR construction (debug operator new + apCL cookie + Resize(1024)). */
#include "fioFILE.h"
#include "../ds/dsVECTOR.h"

// Debug global allocator overload (size, file, line). boundary.
extern void *operator new(size_t size, const char *file, unsigned int line);

// 0(r13) — process-global fio singleton holding the reorder scratch pointer at +16.
extern unsigned char *g_fioSmallDataSingleton; // UNRESOLVED layout — boundary

void fioFILE::WriteData(const void *data, int size, int reorderGain)
{
    const unsigned char *src = (const unsigned char *)data;

    if ( (state.val & FIO_OPEN_BYTE_REORDER) != 0 && reorderGain > 1 )
    {
        static const char *const kFileIoCpp =
            "D:\\Projects\\code\\common\\src.sys\\fileio\\fileio.cpp";

        dsVECTOR<unsigned char, 8> **scratchSlot =
            (dsVECTOR<unsigned char, 8> **)(g_fioSmallDataSingleton + 16);

        if ( !*scratchSlot )
        {
            dsVECTOR<unsigned char, 8> *scratch =
                (dsVECTOR<unsigned char, 8> *)operator new(
                    sizeof(dsVECTOR<unsigned char, 8>), kFileIoCpp, 0x27F);
            if ( scratch )
            {
                scratch->pData = nullptr;
                scratch->nElem = 0;
                scratch->allocated = 0;
                scratch->__cl.file = kFileIoCpp;
                scratch->__cl.line = 639;
                scratch->Resize(1024);
            }
            *scratchSlot = scratch;
        }

        dsVECTOR<unsigned char, 8> *scratch = *scratchSlot;
        if ( size > scratch->nElem )
            scratch->Resize(size);

        unsigned char *dst = scratch->pData;
        const unsigned char *srcGroup = src;
        int elementCount = size / reorderGain;
        if ( elementCount > 0 )
        {
            unsigned char *dstGroupEnd = &dst[reorderGain - 1];
            do
            {
                int halfPairs = reorderGain / 2;
                if ( halfPairs > 0 )
                {
                    const unsigned char *lowSrc = srcGroup;
                    unsigned char *highDst = dstGroupEnd;
                    do
                    {
                        // reverse the byte pair across the group: low<->high
                        // lowSrc+(dst-src) addresses the writable dst buffer; drop const for the store
                        ((unsigned char *)lowSrc)[dst - src] = highDst[src - dst]; // dst_low  = src_high
                        *highDst-- = *lowSrc++;                  // dst_high = src_low
                    }
                    while ( --halfPairs );
                }
                --elementCount;
                srcGroup += reorderGain;
                dst += reorderGain;
                dstGroupEnd += reorderGain;
            }
            while ( elementCount );
        }
        data = scratch->pData;
    }

    DoWriteData(data, size);

    unsigned int prevStreamSize = streamSize;
    unsigned int newOffset = offset + size;
    offset = newOffset;
    if ( newOffset > prevStreamSize )
        streamSize = newOffset;
}
