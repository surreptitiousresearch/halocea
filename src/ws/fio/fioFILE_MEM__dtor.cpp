#include "fioFILE_MEM.h"
#include "fio_boundary.h"

// fioFILE_MEM::~fioFILE_MEM() @ 0x825AE730 — virtual dtor (vtable slot 0). Destroys the
// pending async completion signal (if any), frees the backing memory block when
// self-managed (aligned free iff FIO_OPEN_ALIGN_SECTOR, bit 0x100, is set in `state`),
// resets the memory-stream cursor fields, tears down the async info record via
// osASYNC_INFO::Term, then swaps to the fioFILE vtable before freeing the read/write
// stream buffer (base fioFILE::~fioFILE runs next in the chain).
fioFILE_MEM::~fioFILE_MEM()
{
    osHANDLE_DUMMY *signal = fpAsyncInfo.signal;
    // vftable write reproduced from the decompile (re-asserts fioFILE_MEM's own vtable).
    if (signal)
        osDestroySignal(signal);

    if (isSelfManaged)
    {
        unsigned char *block = pBuf;
        if ((state.val & FIO_OPEN_ALIGN_SECTOR) != 0) // 0x100
            dlFreeAligned(block);
        else
            dlFree(block);
    }

    pBuf = nullptr;
    curPtr = nullptr;
    allocSize = 0;
    streamSize = 0;
    offset = 0;
    fpAsyncInfo.Term();

    fioFILE_BUFFER *streamBuffer = pBuffer;
    // decompile re-points __vftable to fioFILE::`vftable' here, immediately before
    // fioFILE::~fioFILE runs as the next base dtor in the chain.
    if (streamBuffer)
        dlFree(streamBuffer);
}
