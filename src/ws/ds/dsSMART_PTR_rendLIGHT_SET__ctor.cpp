#include "dsSMART_PTR.h"
#include "../rend/rendLIGHT_SET.h"

extern "C" long osLockedIncrement(volatile int *value); // boundary — atomic increment primitive

// dsSMART_PTR<rendLIGHT_SET,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>::dsSMART_PTR(const dsSMART_PTR&) @ 0x825E2D90
// Copy-construct: adopt `rhs`'s pointee and, unless it is null or the (rendLIGHT_SET*)-4 unresolved
// (deferred-load) sentinel, bump the intrusive reference count. The `&ref - 1` cast recovers the
// rendLIGHT_SET* from the embedded dsREF_COUNT& (ref is the field right after the vtable, so this is
// the same pointer, just re-typed — matches the pattern in DeletePointee).
template<>
dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >::dsSMART_PTR(
    const dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > &rhs)
{
    rendLIGHT_SET *rhsPointee = rhs.pointee;
    rendLIGHT_SET *adopted;

    if (!rhsPointee || rhsPointee == (rendLIGHT_SET *)-4) {
        adopted = nullptr;
    } else {
        osLockedIncrement(&rhsPointee->ref.refCount);
        adopted = rhsPointee;
    }
    this->pointee = adopted;
}
