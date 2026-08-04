// ds::WEAK_PTR<entENTITY>::WEAK_PTR() @0x823CF3CC — default-construct: no referent.
#include "../../headers/ws/ds/WEAK_PTR.h"

struct entENTITY; // boundary — only referenced through the handle pointer

template<>
ds::WEAK_PTR<entENTITY>::WEAK_PTR()
{
    pHandle = nullptr;
}

// ds::WEAK_PTR<entENTITY>::WEAK_PTR(T*) @ (calls SetPtr) — construct bound to `pPtr`.
template<>
ds::WEAK_PTR<entENTITY>::WEAK_PTR(entENTITY *pPtr)
{
    pHandle = nullptr;
    SetPtr(pPtr);
}
