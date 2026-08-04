#include "dsCONST_ARRAY.h"
#include "../ap/apMEM_DL_STAT.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsCONST_ARRAY<apMEM_DL_STAT,32>::Remove @ 0x82C4BBA0
// Remove the entry at `idx`: shift the trailing entries down over it, clear the now-unused last
// slot, then drop the entry count. Bounds-checked against both the capacity (32) and the count.
template<>
void dsCONST_ARRAY<apMEM_DL_STAT, 32>::Remove(int idx)
{
    if ((unsigned int)idx >= 0x20u)
        return;
    int nEntry = this->nEntry;
    if (idx >= nEntry)
        return;
    if (idx != nEntry - 1) {
        memmove(&this->list[idx], &this->list[idx + 1], 8 * (nEntry - idx - 1));
        apMEM_DL_STAT *last = &this->Back();
        if (last) {
            last->type = AP_DLSTAT_DL_NA;
            last->size = 0;
        }
    }
    --this->nEntry;
}
