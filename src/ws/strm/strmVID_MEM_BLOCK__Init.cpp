#include "../../headers/ws/strm/strmVID_MEM_BLOCK.h"

// vid subsystem boundary — physical/GPU memory allocator.
// mangled ?vidAllocPhysMem@@YAPAXHIIPAII@Z: arg3 (actualSize) is unsigned int* (PAI).
extern void *vidAllocPhysMem(unsigned long size, unsigned long align, unsigned long flags,
                              unsigned int *actualSize, unsigned long extra);

// strmVID_MEM_BLOCK::Init @ 0x8269CCA0
// Reserve this block's physical-memory range: try a fixed 20 MB (0x1400000) request first; if the
// video driver can't satisfy it, fall back to requesting "whatever is left" (size = 0xFFFFFFFF),
// which reports the actual size it granted back through &this->size. On success, seed the free
// list with a single buffer spanning the whole reservation and record the block id.
bool strmVID_MEM_BLOCK::Init(unsigned int blockId)
{
    this->size = 0x1400000; // 20 MB, the preferred fixed reservation size
    unsigned char *reserved = (unsigned char *)vidAllocPhysMem(0x1400000u, 0x1000u, 0x404u, nullptr, 1u);
    this->offset = reserved;
    if (!reserved) {
        // fallback: take whatever physical memory remains; the driver writes the granted size
        // back into this->size via the actualSize out-param
        this->offset = (unsigned char *)vidAllocPhysMem(0xFFFFFFFFu, 0x1000u, 0x404u, (unsigned int *)&this->size, 1u);
    }
    if (!this->offset)
        return false;

    this->id = blockId;
    // seed the free list with one buffer covering the entire reservation
    AddNewBuffer(1u, this->offset, this->size, 1u, nullptr, nullptr);
    this->freeSize = this->size;
    this->freeBufMaxSize = this->size;
    return true;
}
