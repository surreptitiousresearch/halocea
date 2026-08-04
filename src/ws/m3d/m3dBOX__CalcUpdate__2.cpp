#include "m3dBOX.h"

// m3dBOX::CalcUpdate(const m3dBOX*) @ 0x8265C9A8
// Grow this box to contain another box by folding in both of its corners.
void m3dBOX::CalcUpdate(const m3dBOX *pB)
{
    CalcUpdate(&pB->bll);
    CalcUpdate(&pB->fur);
}
