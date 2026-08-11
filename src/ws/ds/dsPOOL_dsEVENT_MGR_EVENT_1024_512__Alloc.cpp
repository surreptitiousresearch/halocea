#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::EVENT,1024,512>::Alloc @ 0x82616198
// Take the head free slot (growing the pool with a fresh chunk if the free list is empty), unlink
// it, reset its parameter list to empty, and return its index. The free-list link is threaded
// through the slot's leading `id` word.
template<>
int dsPOOL<dsEVENT_MGR::EVENT, 1024, 512>::Alloc()
{
    if (this->emptyChain < 0)
        this->AllocChunk();
    if (!IGNORE_STRONG_ASSERT && this->emptyChain < 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "emptyChain >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            128,
            empty_string);

    int slotIndex = this->emptyChain;
    dsEVENT_MGR::EVENT *slot = &this->Get(slotIndex);
    this->emptyChain = slot->id; // advance past this slot's free-list link

    // reset the pooled event's parameter list to empty
    slot->params.length = 0;
    slot->params.list = nullptr;
    slot->params.storage.pData = nullptr;
    slot->params.storage.nElem = 0;
    slot->params.storage.allocated = 0;
    slot->params.storage.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_param.h";
    slot->params.storage.__cl.line = 98;
    return slotIndex;
}
