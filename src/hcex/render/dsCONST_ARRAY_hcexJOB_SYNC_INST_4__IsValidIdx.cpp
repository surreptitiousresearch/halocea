#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/hcex/hcexJOB_SYNC_INST.h"

// dsCONST_ARRAY<hcexJOB_SYNC_INST,4>::IsValidIdx @ 0x823CFA70
// ?IsValidIdx@?$dsCONST_ARRAY@VhcexJOB_SYNC_INST@@$03@@QBAHH@Z
// True only when idx is within the fixed capacity (N=4) AND below the live-entry count.
template<>
int dsCONST_ARRAY<hcexJOB_SYNC_INST, 4>::IsValidIdx(int idx) const
{
    if ((unsigned int)idx >= 4)
        return 0;
    if (idx >= this->nEntry)
        return 0;
    return 1;
}
