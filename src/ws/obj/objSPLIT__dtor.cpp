#include "objSPLIT.h"

extern "C" void dlFree(void *ptr);        // boundary — dlmalloc allocator free
extern "C" void dlFreeAligned(void *ptr); // boundary — dlmalloc aligned-block free

// objSPLIT::~objSPLIT() @ 0x82BC81C8
// Release every live entry's material table and its (aligned) SPU config block, then free the
// entry array itself.
objSPLIT::~objSPLIT()
{
    for (int entryIndex = 0; entryIndex < this->nEntry; ++entryIndex) {
        objSPLIT_ENTRY *entry = &this->entryList[entryIndex];
        entry->mtlTbl.Clear();
        dlFreeAligned(entry->spuConfig);
    }
    dlFree(this->entryList);
}
