#include "STATE.h"

namespace sml {

// ?Set@STATE@sml@@QAAXABV?$dsTSTRING@D@@@Z (@0x82666510) — set the active state by NAME:
// resolve `nameState` to a numeric state id via the owning actor's descriptor
// (iaDESC::RegisterSmlState), then forward to the Set(int) overload to do the actual
// bit-set bookkeeping.
void STATE::Set(const dsTSTRING<char> &nameState)
{
    iaDESC *ownerDesc = Desc<iaIACTOR>(pOwner);
    int idxState = ownerDesc->RegisterSmlState(nameState);
    Set(idxState);
}

} // namespace sml
