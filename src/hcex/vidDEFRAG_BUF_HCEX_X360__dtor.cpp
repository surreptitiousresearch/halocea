#include "../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"

// vidDEFRAG_BUF_HCEX_X360::~vidDEFRAG_BUF_HCEX_X360 @ 0x823F1690
// No fields of its own to release (pIBuf/pVBuf are owned by CreateBuf's allocation, not freed
// here — reproduced as-is from the DB). Empty body; the base-class chain
// (~vidDEFRAG_BUF -> ~apDEFRAG_BUF, both boundary/not reversed here) runs automatically and frees
// the four dsVECTOR backing arrays (chunks/freeBlocks/chunksRT/usedBlocks) via their own dtors —
// matches the disasm, which shows this compiled straight through to four dlFree calls with no
// intervening call instruction (both base levels were fully inlined by the compiler).
vidDEFRAG_BUF_HCEX_X360::~vidDEFRAG_BUF_HCEX_X360()
{
}
