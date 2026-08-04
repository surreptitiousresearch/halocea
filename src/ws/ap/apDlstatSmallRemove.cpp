// apDlstatSmallRemove @ 0x82C4C528 — locate (and "remove", by rewinding the row's free-slot
// hint) the small-allocation statistics record for `memBlock` in the _apMemStat hash table.
// Bucketing matches apDLSTAT_SMALL_IFACE::Add: row = (memBlock >> 10) & 0x7F, 1024 slots per row.
// Fast path checks the last-used slot first; otherwise it linearly scans the row. Returns the
// matching record, or null if the block is not tracked.
#include "../../headers/ws/ap/apMEM_STAT_HASH.h"

apMEM_ALLOC_INFO *apDlstatSmallRemove(unsigned int memBlock)
{
    int row = (memBlock >> 10) & 0x7F;
    apMEM_ALLOC_INFO *rowSlots = _apMemStat->allocInfo[row];
    int used = _apMemStat->free_block[row];
    int last = used - 1;

    // fast path: the block is most often the most-recently-added slot in its row
    if (last > 0 && rowSlots[last].memBlock == (void *)memBlock)
    {
        if (used > last)
            _apMemStat->free_block[row] = last;
        return &rowSlots[last];
    }

    // linear scan of the row's 1024 slots
    for (int i = 0; i < 1024; ++i)
    {
        if ((unsigned int)(size_t)rowSlots[i].memBlock == memBlock)
        {
            if (used > i)
                _apMemStat->free_block[row] = i;
            return &rowSlots[i];
        }
    }
    return nullptr;
}
