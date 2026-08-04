#include "fioFILE_MEM.h"
#include "fio_boundary.h"
// fioFILE_MEM::InitMemory(void *, unsigned int, dsFLAGS<FIO_OPEN,int>) @ 0x825AE7E0 —
// bind the stream to a caller-supplied memory block (or, if null, mark it self-managed
// for later allocation). Releases any previously self-owned block and chunk list first.
// Returns 1 for a valid read/write mode, 0 otherwise.

int fioFILE_MEM::InitMemory(void *buffer, unsigned int buflen, dsFLAGS<FIO_OPEN,int> state_)
{
    if ( isSelfManaged )
    {
        unsigned int flags = state.val;
        unsigned char *block = pBuf;
        if ( (flags & FIO_OPEN_ALIGN_SECTOR) != 0 )
            dlFreeAligned(block);
        else
            dlFree(block);
    }

    fioCHUNK_DESCR *chunkList = descrList;
    state.val = state_.val;
    pBuf = nullptr;
    curPtr = nullptr;
    allocSize = 0;
    streamSize = 0;
    offset = 0;
    if ( chunkList )
        dlFree(chunkList);
    nDescr = 0;
    nDescrAlloc = 0;
    descrList = nullptr;
    SetBuffer(0);

    pBuf = (unsigned char *)buffer;
    allocSize = buflen;
    if ( !buffer )
        allocSize = 0;

    unsigned int flags = state.val;
    isSelfManaged = (buffer == nullptr);
    if ( (flags & FIO_OPEN_W) != 0 )
    {
        streamSize = 0;
        curPtr = (unsigned char *)buffer;
        offset = 0;
        return 1;
    }
    else if ( (flags & FIO_OPEN_R) != 0 )
    {
        streamSize = buflen;
        curPtr = (unsigned char *)buffer;
        offset = 0;
        return 1;
    }
    return 0;
}
