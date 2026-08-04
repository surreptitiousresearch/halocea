#include "../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"

// vidDEFRAG_BUF_HCEX_X360::RemoveChunk @ 0x823F1680 (thunk)
// Thunks straight through to the generic apDEFRAG_BUF implementation (boundary — not reversed
// here).
void vidDEFRAG_BUF_HCEX_X360::RemoveChunk(apDEFRAG_CHUNK *pChunk)
{
    apDEFRAG_BUF::RemoveChunk(pChunk);
}
