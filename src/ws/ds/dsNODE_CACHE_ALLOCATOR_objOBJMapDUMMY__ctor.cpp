#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::dsNODE_CACHE_ALLOCATOR(apCL) @ 0x82689FE8
// Forwards to the dsNODE_CACHE(apCL) base ctor (no own fields). DEVIATION: decompile flagged
// "local variable allocation has failed"; field-by-field writes re-derived against the
// DB-verified nNode@0/nodeList@4/freeNodeList@24/__cl@36 layout, identical to the base ctor's.
template<>
dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::dsNODE_CACHE_ALLOCATOR(apCL cl)
    : dsNODE_CACHE<dsObjOBJVecMapDUMMY>(cl)
{
}
