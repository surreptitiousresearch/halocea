#include "fioFILE_MEM.h"
#include "fio_boundary.h"
// fioFILE_MEM::TermMemory() @ 0x825AD930 — release a self-managed backing block (via the
// aligned or plain free per FIO_OPEN_ALIGN_SECTOR) and reset the memory-stream cursor.

void fioFILE_MEM::TermMemory()
{
    if ( isSelfManaged )
    {
        unsigned int flags = state.val;
        unsigned char *block = pBuf;
        if ( (flags & FIO_OPEN_ALIGN_SECTOR) != 0 ) // 0x100
            dlFreeAligned(block);
        else
            dlFree(block);
    }
    pBuf = nullptr;
    curPtr = nullptr;
    allocSize = 0;
    streamSize = 0;
    offset = 0;
}
