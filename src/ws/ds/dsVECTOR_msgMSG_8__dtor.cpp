#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<msgMSG,8>::~dsVECTOR @ 0x825576A0
// Deviation: disasm confirms the per-element loop body is genuinely empty (same as Clear) — no
// DelInfo teardown, just free the backing storage.
template<>
dsVECTOR<msgMSG, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
