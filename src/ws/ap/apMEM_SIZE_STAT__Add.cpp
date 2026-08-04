// apMEM_SIZE_STAT::Add @ 0x826C09E0
// Account for a new allocation: bump the grand totals and the per-bucket totals,
// then advance the high-water marks.
// The decompiler open-coded the sizes[]/count[] indexing as pointer math off
// &sizeTotal; it resolves to sizes[type] (base 16) and count[type] (base 32).
#include "apMEM_SIZE_STAT.h"

void apMEM_SIZE_STAT::Add(int size, apMEM_BLOCK_TYPE type)
{
    this->sizeTotal += size;
    this->countTotal += 1;

    this->sizes[type] += size;
    this->count[type] += 1;

    if (this->sizeTotalMax < this->sizeTotal)
        this->sizeTotalMax = this->sizeTotal;

    if (this->countTotalMax < this->countTotal)
        this->countTotalMax = this->countTotal;
}
