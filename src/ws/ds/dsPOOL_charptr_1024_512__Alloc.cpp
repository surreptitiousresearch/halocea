#include "dsPOOL.h"
#include "ds_assert_boundary.h"

// dsPOOL<char const*,1024,512>::Alloc @ 0x82529268
// Take the head free slot (growing the pool with a new chunk if the free list is empty),
// unlink it, store `value` into it, and return its index.
template<>
int dsPOOL<const char *, 1024, 512>::Alloc(const char *const &value)
{
    if (this->emptyChain < 0)
        this->AllocChunk();
    if (!IGNORE_STRONG_ASSERT && this->emptyChain < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "emptyChain >= 0",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
            147,
            empty_string);

    int slotIndex = this->emptyChain;
    // Free slots overlay an int "next free index"; the pool element storage is that same word.
    int *slot = (int *)this->Get(slotIndex);
    this->emptyChain = *slot;     // advance the free list past this slot
    *slot = (int)value;           // write the payload pointer into the now-live slot
    return slotIndex;
}
