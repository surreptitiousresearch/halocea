#include "fioFILE_MEM.h"
// fioFILE_MEM::Term() @ 0x825B05F8 (vtable slot 2 override) — run the base finalizer,
// but for a writable in-memory stream preserve the accumulated size/position (so a
// written buffer can still be read back after finalization).

int fioFILE_MEM::Term()
{
    unsigned int savedStreamSize = streamSize;
    unsigned int savedOffset = offset;
    unsigned int wasWritable = (state.val >> 1) & 1; // FIO_OPEN_W
    int baseResult = fioFILE::Term();
    if ( wasWritable )
    {
        streamSize = savedStreamSize;
        offset = savedOffset;
    }
    return baseResult;
}
