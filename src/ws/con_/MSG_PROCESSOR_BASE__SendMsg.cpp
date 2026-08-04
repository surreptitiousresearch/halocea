#include "MSG_PROCESSOR_BASE.h"
#include "../ap/apProfileTimer.h"

// os primitives -- boundary (inline extern, matching sibling ws/* convention).
extern "C" unsigned __int64 osGetPerfCounter(void); // boundary
extern "C" void             osSleep(int ms);        // boundary

namespace con {

// 0x8309B380 -- frame `data`/`datalen` and append to outBuf.
// CAVEAT: the decompile builds the header by numerically byte-swapping datalen and id
// (HIBYTE/BYTE1/BYTE2/LOBYTE reassignment = a 32-bit bswap) into scratch ints, then stores
// those ints to memory with a plain int store. This binary is PPC (big-endian): a big-endian
// store of a bswap32'd value produces little-endian bytes in outBuf. Reconstructed here as
// explicit little-endian byte writes of datalen then id (the wire header format expected by
// whatever reads outBuf, presumably a little-endian PC-side console tool), which is
// equivalent to the decompiled bswap+store sequence. apProfileTimer usage (startTime/
// totalTime) is a throwaway local profiling scratch not stored anywhere observable -- kept
// as a local temporary, matching the decompiler's dead-store pattern.
void MSG_PROCESSOR_BASE::SendMsg(const void *data, int datalen, int id)
{
    apProfileTimer profileScratch;
    profileScratch.totalTime = 0;
    profileScratch.startTime = osGetPerfCounter();

    if (!isConnected || datalen > 0x100000)
        return;

    if (id != 0) {
        // Throttle: wait until outBuf has room for datalen more bytes.
        for (;;) {
            lock.Lock(nullptr, 0);
            int outBufLen = outBuf.nElem;
            lock.Unlock(nullptr, 0);
            if (outBufLen + datalen < 0x100000)
                break;
            osSleep(1);
        }
    }

    // Wire header: little-endian byte order of datalen followed by id.
    unsigned char lengthHeader[4];
    lengthHeader[0] = (unsigned char)(datalen);
    lengthHeader[1] = (unsigned char)(datalen >> 8);
    lengthHeader[2] = (unsigned char)(datalen >> 16);
    lengthHeader[3] = (unsigned char)(datalen >> 24);

    unsigned char idHeader[4];
    idHeader[0] = (unsigned char)(id);
    idHeader[1] = (unsigned char)(id >> 8);
    idHeader[2] = (unsigned char)(id >> 16);
    idHeader[3] = (unsigned char)(id >> 24);

    lock.Lock(nullptr, 0);
    outBuf.Insert(outBuf.nElem, lengthHeader, 4);
    outBuf.Insert(outBuf.nElem, idHeader, 4);
    outBuf.Insert(outBuf.nElem, (const unsigned char *)data, datalen);
    profileScratch.totalTime = osGetPerfCounter() - profileScratch.startTime;
    profileScratch.GetResSec();
    lock.Unlock(nullptr, 0);
}

} // namespace con
