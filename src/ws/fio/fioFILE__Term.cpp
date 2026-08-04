#include "fioFILE.h"
#include "fio_boundary.h"
// fioFILE::Term() @ 0x825AF9A8 (vtable slot 2, base implementation) — finalize the stream.
// For a chunked writable stream (FIO_OPEN_W|FIO_OPEN_CHUNK, unless FIO_OPEN_NO_OFFSET_END),
// back-patch each chunk's end offset, then release the buffer and chunk list and reset
// all bookkeeping. Returns 1 when no FIO_ERROR bit is set.

int fioFILE::Term()
{
    unsigned int flags = state.val;
    if ( (flags & FIO_OPEN_W) != 0
      && (flags & FIO_OPEN_CHUNK) != 0
      && (flags & FIO_OPEN_NO_OFFSET_END) == 0 )
    {
        for ( int i = 0; i < nDescr; ++i )
        {
            fioFILE_BUFFER *buf = pBuffer;
            offset = descrList[i].offsetStart + 2;
            if ( !buf )
                DoSeek(offset); // decompiler dropped the offset arg; slot takes (pos)
            int chunkEnd = descrList[i].offsetEnd;
            WriteData(&chunkEnd, 4, 4);
        }
    }

    SetBuffer(0);
    fioCHUNK_DESCR *chunkList = descrList;
    state.val = 0;
    streamSize = 0;
    offset = 0;
    nDescr = 0;
    nDescrAlloc = 0;
    if ( chunkList )
        dlFree(chunkList);
    int finalFlags = state.val;
    descrList = nullptr;
    return ((unsigned int)~finalFlags >> 6) & 1; // 1 == FIO_ERROR (0x40) clear
}
