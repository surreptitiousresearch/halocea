#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::Alloc() @ 0x8268AB90
// Thunk straight to the base dsNODE_CACHE::GetEmpty.
template<>
dsObjOBJVecMapDUMMY *dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::Alloc()
{
    return this->GetEmpty();
}
