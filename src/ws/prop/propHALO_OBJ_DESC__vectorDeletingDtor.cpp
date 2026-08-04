#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::`vector deleting destructor' @ 0x823EBB88
// Compiler-generated deleting-destructor entry point: runs the destructor, then conditionally
// calls operator delete(this) when bit 0 of deleteFlags is set (the "delete this too" case used
// when destroying through a virtual/deleting-dtor call). DB shows only the (this, flags) shape,
// no array-count parameter, despite the "vector" name (same convention as
// vidDEFRAG_POOL_HCEX_X360::vectorDeletingDtor).
propHALO_OBJ_DESC *propHALO_OBJ_DESC::vectorDeletingDtor(unsigned char deleteFlags)
{
    this->~propHALO_OBJ_DESC();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
