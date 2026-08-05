#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::PENDING_ITEM,256,64>::Alloc @ 0x82614998
// Take the head free slot (growing the pool with a new chunk if the free list is empty) and
// return its index; the free-list link is threaded through the slot's idxEvent word.
template<>
int dsPOOL<dsEVENT_MGR::PENDING_ITEM, 256, 64>::Alloc()
{
    if (this->emptyChain < 0)
        this->AllocChunk();
    if (!IGNORE_STRONG_ASSERT && this->emptyChain < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "emptyChain >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            128,
            empty_string);

    int slotIndex = this->emptyChain;
    this->emptyChain = this->Get(slotIndex).idxEvent; // advance past this slot's free-list link
    return slotIndex;
}
