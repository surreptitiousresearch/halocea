#include "dsPOOL.h"
#include "dsSTATE_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsSTATE_MGR::STATE_REC,256,256>::Alloc @ 0x82614CE0
// Take the head free slot (growing the pool with a new chunk if the free list is empty) and
// return its index; the free-list link is threaded through the slot's idxData word.
template<>
int dsPOOL<dsSTATE_MGR::STATE_REC, 256, 256>::Alloc()
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
    this->emptyChain = this->Get(slotIndex).idxData; // advance past this slot's free-list link
    return slotIndex;
}
