#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Clear @ 0x82AC3C00
// Deviation: disasm confirms the per-element loop body is genuinely empty (no DelInfo call) —
// this only resets the live count, despite msgMSG owning a heap payload via DelInfo elsewhere.
template<>
void dsVECTOR<msgMSG, 8>::Clear()
{
    this->nElem = 0;
}
