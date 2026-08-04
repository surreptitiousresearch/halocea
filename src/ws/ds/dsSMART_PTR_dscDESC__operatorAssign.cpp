#include "dsSMART_PTR.h"
#include "../dsc/dscDESC.h"

extern "C" long osLockedIncrement(volatile int *value); // boundary — atomic increment primitive

// dsSMART_PTR<dscDESC,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>::operator= @ 0x825D6480
// Self-assignment no-op; otherwise release the current referent, then adopt rhs's pointee
// (bumping its intrusive count unless it is null or the -4 unresolved sentinel).
template<>
dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > &
dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >::operator=(
    const dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > &rhs)
{
    if (this != &rhs) {
        this->DeletePointee();

        dscDESC *rhsPointee = rhs.pointee;
        dscDESC *adopted;
        if (!rhsPointee) {
            adopted = nullptr;
        } else {
            osLockedIncrement(&rhsPointee->ref.refCount);
            adopted = rhsPointee;
        }
        this->pointee = adopted;
    }
    return *this;
}
