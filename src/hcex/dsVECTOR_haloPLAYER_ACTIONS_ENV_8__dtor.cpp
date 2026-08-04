#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

// dsVECTOR<haloPLAYER_ACTIONS_ENV,8>::~dsVECTOR() @ 0x823BE9B0
// haloPLAYER_ACTIONS_ENV is POD, so the per-element destructor loop is empty (retained from the
// reversed body); then free the backing store. Reversed per-instantiation.
template<>
dsVECTOR<haloPLAYER_ACTIONS_ENV, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // POD element — trivial destructor
    dlFree(this->pData);
}
