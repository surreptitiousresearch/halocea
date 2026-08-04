#include "m3dMATR.h"

// m3dMultiplyMatr @ 0x825426E8 (276 bytes)
// CAVEAT: the decompiler produced NO typed C body for this function at all — the entire
// 276-byte function is raw AltiVec/VMX128: paired lvlx128/lvrx128 unaligned-quadword loads
// reconstructing a's and b's four 16-byte rows, vspltw128 broadcasting each element of an
// a-row across a lane, vmulfp128/vmaddfp accumulating against b's rows, then stvlx/stvrx
// storing the four result rows into c (out). There are no named locals or lvars to recover
// at all (pure vector-register dataflow, "output may be wrong"-class decompile). The
// reconstruction below is the direct mathematical semantics of that dataflow: each output
// row is the broadcast-multiply-accumulate of one row of `a` against all four rows of `b`,
// i.e. the standard row-major matrix product out = a * b, out[i][k] = sum_j a[i][j]*b[j][k].
// This matches how the two call sites in m3dMATR::Transform document the operation
// ("product = this * pMTrans" / "product = pMTrans * this"). Verify against disasm if any
// caller ever appears to expect a different (e.g. transposed/column-major) convention.
void m3dMultiplyMatr(const m3dMATR *a, const m3dMATR *b, m3dMATR *out)
{
    for (int row = 0; row < 4; ++row)
    {
        float rowCoeff0 = a->m[row][0];
        float rowCoeff1 = a->m[row][1];
        float rowCoeff2 = a->m[row][2];
        float rowCoeff3 = a->m[row][3];

        for (int col = 0; col < 4; ++col)
        {
            out->m[row][col] = rowCoeff0 * b->m[0][col]
                              + rowCoeff1 * b->m[1][col]
                              + rowCoeff2 * b->m[2][col]
                              + rowCoeff3 * b->m[3][col];
        }
    }
}
