#include "dsSMART_PTR.h"
#include "../rend/rendLIGHT_SET.h"

extern "C" long osLockedIncrement(volatile int *value); // boundary — atomic increment primitive

// dsSMART_PTR<rendLIGHT_SET,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>::operator= @ 0x825E3828
// Self-assignment no-op; otherwise release the current referent, then adopt rhs's pointee
// (bumping its intrusive count unless it is null or the -4 unresolved sentinel).
template<>
dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > &
dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >::operator=(
    const dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > &rhs)
{
    if (this != &rhs) {
        this->DeletePointee();

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
    return *this;
}
