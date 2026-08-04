#include "dsSMART_PTR.h"
#include "../rend/rendLIGHT_SET.h"

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement primitive

// dsSMART_PTR<rendLIGHT_SET,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>::DeletePointee @ 0x8253D3C0
// Release this owner's reference on the light set. The (rendLIGHT_SET*)-4 value is an unresolved
// (deferred-load) sentinel and owns nothing. Otherwise atomically drop the intrusive count and,
// when this owner held the last reference, invoke the virtual deleting destructor; then null out.
template<>
void dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >::DeletePointee()
{
    rendLIGHT_SET *pointee = this->pointee;
    if (pointee) {
        if (pointee != (rendLIGHT_SET *)-4) {
            int locked = osLockedDecrement(&pointee->ref.refCount);
            // last reference: both `locked` and `locked-1` have their sign bit set
            if ((locked | (locked - 1)) < 0) {
                if (this->pointee)
                    // scalar-deleting dtr: deleteFlag=1 => run ~rendLIGHT_SET then free
                    this->pointee->__vftable->dtr_rendLIGHT_SET(this->pointee, 1);
            }
        }
    }
    this->pointee = nullptr;
}
