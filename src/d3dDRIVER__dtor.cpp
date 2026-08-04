#include "headers/d3d_driver.h"

// Boundary: allocator free used to release the two vector backing stores.
extern "C" void dlFree(void *ptr);

// 0x8268F228 -- d3dDRIVER destructor. Reinstalls the d3dDRIVER vtable (compiler-generated for
// the base-in-dtor ordering), tears down the driver object, frees the query/RT-pool vector
// storage, then chains the vidDRIVER base destructor.
d3dDRIVER::~d3dDRIVER()
{
    this->base.__vftable = (vidDRIVER_vtbl *)&d3dDRIVER_vftable;
    this->TermDrv();
    dlFree(this->queryList.pData);
    dlFree(this->RTPool.pData);
    this->base.~vidDRIVER(); // base-subobject dtor chain (composition model) (2026-07-31)
}
