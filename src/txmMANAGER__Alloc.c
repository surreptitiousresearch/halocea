#include "headers/ws/txm/txmMANAGER.h"

// 0x826FB0CC
txmTEXTURE *txmMANAGER::Alloc(const char *name, int texFlags)
{
    txmTEXTURE *tex = this->MakeTex(name, texFlags);
    if (!tex)
        return tex;

    if (this->listNeedSort)
    {
        this->listTex.PushBack(tex);
    }
    else
    {
        txmCMP_TEX_SORT sortCmp;
        this->listTex.InsertSorted<txmCMP_TEX_SORT>(tex, sortCmp, dsVECTOR<txmTEXTURE *, 8>::INS_DUP_IGNORE);
    }

    return tex;
}
