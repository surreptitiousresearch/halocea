#include "animINST.h"

// animINST::_GetMatrL2W @ 0x825E5408
// Fetch the local-to-world matrix and post-apply the per-instance scale (local/row scaling,
// oper == 1).
void animINST::_GetMatrL2W(m3dMATR *pMatL2W)
{
    this->GetMatrL2W(pMatL2W);
    // Disasm sets r7 = 1 for the int `oper` arg; the decompiler emitted an uninitialised
    // temporary here — the real value is the literal 1.
    pMatL2W->Scale(this->scale.x, this->scale.y, this->scale.z, 1);
}
