#include "scnSCENE.h"

// ?FindNextInst@scnSCENE@@QAAPAVanimINST@@PAV2@@Z  (0x8253C718) — the instance following `pInst`
// in the live list, or the list head when `pInst` is null.
animINST *scnSCENE::FindNextInst(animINST *pInst)
{
    if (!pInst)
        return this->pFirstInst;
    return pInst->next;
}
