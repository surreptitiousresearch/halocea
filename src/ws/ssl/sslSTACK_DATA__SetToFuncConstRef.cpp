/* ?SetToFuncConstRef@sslSTACK_DATA@@QAAAAV1@PAVdsDATA@@H@Z @0x82AF0930 */
#include "sslSTACK_DATA.h"

// 0x82AF0930 — sslSTACK_DATA::SetToFuncConstRef. Stores a (constant-table, index) reference into
// the funcConst arm.
sslSTACK_DATA &sslSTACK_DATA::SetToFuncConstRef(dsDATA *pTbl, int idx)
{
    Cleanup();
    funcConst.pTbl = pTbl;
    funcConst.idx = idx;
    type = TYPE_SSLFUNC_CONST_REF;
    return *this;
}
