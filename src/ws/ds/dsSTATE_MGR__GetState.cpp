#include "dsSTATE_MGR.h"
#include "dsDATA.h"

// dsSTATE_MGR::GetState @ 0x82615D80
// Resolve state `id` to its stored value. If the state record points at a live value slot,
// return it; otherwise return a shared, lazily-initialised empty dsDATA.
const dsDATA *dsSTATE_MGR::GetState(int id)
{
    int idxData = this->stateIndex.Get(id).idxData;
    if (idxData >= 0)
        return &this->states.Get(idxData);

    // Function-local empty value; the binary guards a one-time init (type = null) with atexit.
    static dsDATA nullData{}; // value-init: storage=0, type=null (dsDATA is not an aggregate)
    return &nullData;
}
