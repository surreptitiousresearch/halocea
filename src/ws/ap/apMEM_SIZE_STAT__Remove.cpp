// apMEM_SIZE_STAT::Remove @ 0x826C0A50
// Reverse an allocation: drop the grand totals and the per-bucket totals. The
// max fields are still re-checked afterward, mirroring the shared code path.
// sizes[type] (base 16) / count[type] (base 32) — same open-coded indexing as Add.
#include "apMEM_SIZE_STAT.h"

void apMEM_SIZE_STAT::Remove(int size, apMEM_BLOCK_TYPE type)
{
    this->sizeTotal -= size;
    this->countTotal -= 1;

    this->sizes[type] -= size;
    this->count[type] -= 1;

    if (this->sizeTotalMax < this->sizeTotal)
        this->sizeTotalMax = this->sizeTotal;

    if (this->countTotalMax < this->countTotal)
        this->countTotalMax = this->countTotal;
}
