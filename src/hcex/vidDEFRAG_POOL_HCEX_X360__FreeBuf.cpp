#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"

// vidDEFRAG_POOL_HCEX_X360::FreeBuf @ 0x823F13B4
// Buffers in this pool are backed by a single physical-memory reservation (see AllocBuf) that is
// never returned, so freeing an individual buffer is a no-op: always report failure.
bool vidDEFRAG_POOL_HCEX_X360::FreeBuf(apDEFRAG_BUF *buf)
{
    return false;
}
