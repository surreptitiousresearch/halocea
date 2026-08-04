#include "../../headers/ws/strm/strmRES_TEX_MANAGER.h"

// strmRES_TEX_MANAGER::Subupdate @ 0x8266EC20
// Walk mip levels downward from *checkMip, trying up to 3 candidate budgets in order
// (memHi, memMid, memLo): for each candidate mip, size it (rounded up to a 4KB page) and check
// whether it fits under the corresponding budget pointer. On the first fit, debit that budget and
// *totalMem, write the accepted mip back through *checkMip, and return. On a miss, step the mip
// down by one (clamped to minMip) and try the next budget; if minMip is reached before the third
// budget is tried, accept it immediately. If all three budgets are exhausted without a fit,
// *checkMip is left at minMip.
int strmRES_TEX_MANAGER::Subupdate(strmRESOURCE_TEXTURE *strmResource, int *totalMem, int *memHi,
                                    int *memMid, int *memLo, int *rest, int *checkMip)
{
    int *budgets[3] = { memHi, memMid, memLo };
    int mip = *checkMip;
    int budgetIdx = 0;
    int minMip = strmResource->minMip;

    for (;;) {
        int size = strmResource->CalcTexureSize(
            strmResource->header.sx >> (strmResource->maxMip - mip),
            strmResource->header.sy >> (strmResource->maxMip - mip),
            strmResource->header.nMipMap - strmResource->maxMip + mip);

        if (size % 4096)
            size = (size / 4096 + 1) << 12;

        if (*budgets[budgetIdx] - size > 0) {
            *budgets[budgetIdx] -= size;
            *totalMem -= size;
            *checkMip = mip;
            return 0;
        }

        minMip = strmResource->minMip;
        --mip;
        ++budgetIdx;
        if (mip <= minMip)
            mip = strmResource->minMip;
        if (budgetIdx > 2)
            break;
        if (mip == minMip) {
            *checkMip = mip;
            return 0;
        }
    }

    *checkMip = minMip;
    return 0;
}
