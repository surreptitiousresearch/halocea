#include "dsPOOL.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsDATA,256,256>::Alloc @ 0x82614D80
// Take the head free slot (growing the pool with a new chunk if the free list is empty) and
// return its index. The free-list link is threaded through the slot's `storage` word; after
// unlinking, the slot is reset to the empty dsDATA (null type descriptor).
template<>
int dsPOOL<dsDATA, 256, 256>::Alloc()
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
    dsDATA *slot = &this->Get(slotIndex);
    this->emptyChain = slot->storage; // advance past this slot's free-list link
    slot->type = nullptr;
    return slotIndex;
}
