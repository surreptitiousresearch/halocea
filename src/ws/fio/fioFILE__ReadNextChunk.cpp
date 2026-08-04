#include "fioFILE.h"

// fioFILE::ReadNextChunk(fioCHUNK *) @ 0x825ADCE8 — read the next chunk header via
// ReadChunk, then report whether iteration should continue. ReadChunk returns 1 at
// physical EOF (→ 0 here); otherwise the FIO_OPEN state's bit 6 (error) or bit 7
// (logical eof) also stop iteration. Returns 1 only when another chunk is available.

int fioFILE::ReadNextChunk(fioCHUNK *pChnk)
{
    if ( ReadChunk(pChnk) == 1 )
        return 0;

    unsigned int val = state.val;
    if ( ((val >> 6) & 1) != 0 )   // FIO error flag
        return 0;
    if ( ((val >> 7) & 1) != 0 )   // FIO eof flag
        return 0;
    return 1;
}
