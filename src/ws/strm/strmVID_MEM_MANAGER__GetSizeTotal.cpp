#include "../../headers/ws/strm/strmVID_MEM_MANAGER.h"

// strmVID_MEM_MANAGER::GetSizeTotal @ 0x8269D538
// Sum the `size` of every block in `blocks`, allocating a first block via AllocateNewBlock if
// none exist yet.
int strmVID_MEM_MANAGER::GetSizeTotal()
{
    if (blocks.nElem == 0)
        AllocateNewBlock();

    int total = 0;
    for (int i = 0; i < blocks.nElem; ++i)
        total += blocks.pData[i]->size;

    return total;
}
