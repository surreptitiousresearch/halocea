#include "../../headers/ws/prop/propHALO_OBJ.h"

// propHALO_OBJ::`vector deleting destructor' @ 0x823EC348
// Compiler-generated deleting-destructor entry point: runs the destructor, then conditionally
// calls operator delete(this) when bit 0 of deleteFlags is set. DB shows only the (this, flags)
// shape, no array-count parameter, despite the "vector" name (same convention as
// vidDEFRAG_POOL_HCEX_X360::vectorDeletingDtor).
propHALO_OBJ *propHALO_OBJ::vectorDeletingDtor(unsigned char deleteFlags)
{
    this->~propHALO_OBJ();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
