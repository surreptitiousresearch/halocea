#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::Transform @ 0x82542C28
// Combine this matrix with another, selected by `oper`:
//   0: copy pMTrans into this;
//   1: this = pMTrans * this;
//   2: this = this * pMTrans.
// CAVEAT: the decompiler flagged "local variable allocation has failed" and rendered the
// 64-byte matrix copies as OVERLAPPED __int128/__int64 pointer packing (this stashed in
// HIDWORD/DWORD1 slots). Reconstructed here as whole-struct assignment — the union copies
// all 16 elements.
void m3dMATR::Transform(const m3dMATR *pMTrans, int oper)
{
    if (oper == 0)
    {
        *this = *pMTrans;
    }
    else
    {
        m3dMATR product;
        if (oper == 2)
            m3dMultiplyMatr(this, pMTrans, &product);   // product = this * pMTrans
        else
            m3dMultiplyMatr(pMTrans, this, &product);   // product = pMTrans * this
        *this = product;
    }
}
