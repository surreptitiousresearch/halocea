#include "dsDEQUE.h"
#include "ds_assert_boundary.h"

// boundary — dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::ERASED,
// a deferred-erase bookkeeping record for the int-valued string map. Opaque here.
struct dsStrIntMapERASED;

// dsDEQUE<...int-map ERASED>::Back @ 0x826169D8
// Reference to the last live element (the slot at index `tail`); assert non-empty first.
template<>
dsStrIntMapERASED *dsDEQUE<dsStrIntMapERASED>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->length <= 0)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "Length() > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_deque.hpp",
            318,
            dsStrongAssertMessage);
    return &this->data[this->tail];
}
