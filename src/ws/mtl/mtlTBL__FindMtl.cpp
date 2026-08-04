#include "mtlTBL.h"

// mtlTBL::FindMtl @ 0x82C2CC98
// CAVEAT: disassembly-confirmed deviation from the mangled signature — the body ignores
// its own startPos/direction parameters and always calls _FindMtl(mtl, 0, 0) (0x82C2CCA8/
// 0x82C2CCAC hardcode r5=0, r6=0 into the call regardless of the incoming args). Reproduced
// faithfully rather than "fixed", since that is what the shipped binary does.
mtlMTL *mtlTBL::FindMtl(int mtl, int startPos, int direction)
{
    int found_index = this->_FindMtl(mtl, 0, 0);
    if (found_index == -1)
        return nullptr;
    return &this->mtlList[found_index];
}
