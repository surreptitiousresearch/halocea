#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::ITEM_REC,256,256>::Alloc @ 0x82615E10
// Take the head free slot (growing the pool with a new chunk if the free list is empty) and
// return its index. The free-list link is threaded through the record's name.id word, so after
// unlinking, the name dsSTRID is default-constructed to leave the live slot in a clean state.
template<>
int dsPOOL<dsEVENT_MGR::ITEM_REC, 256, 256>::Alloc()
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
    dsEVENT_MGR::ITEM_REC *rec = &this->Get(slotIndex);
    this->emptyChain = *reinterpret_cast<int *>(&rec->name); // free-list link overlays name.id
    rec->name = dsSTRID();                                   // default-construct the interned name
    return slotIndex;
}
