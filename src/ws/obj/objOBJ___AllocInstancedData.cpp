/* ?_AllocInstancedData@objOBJ@@QAAXGE_K@Z @0x82A80008 */
#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "obj_boundary.h"
// 0x82A80008  ?_AllocInstancedData@objOBJ@@QAAXGE_K@Z
// Allocate the per-instance data buffer for this object's shared geometry: free the old buffer, alloc
// num*size bytes (16-byte aligned), and record instance count/size and the instanced FVF. Clears the
// OBJ_ST_SHARED_INSTANCED flag (0x200000). Routes through the dataAlloc/dataFree hooks when set.

void objOBJ::_AllocInstancedData(uint16_t num, uint8_t size, uint64_t fvf)
{
    if (!this->pGeom) {
        // STRONG_ASSERT: 0  (Obj_alloc.cpp:1350)
        return;
    }

    // STRONG_ASSERT: num > 0 && size > 0  (Obj_alloc.cpp:1354)
    _UnshareGeom();

    void *oldInst = this->pGeom->pSharedGeom->pDataInst;
    if (objOBJ::dataFree)
        objOBJ::dataFree(this, oldInst, 35);
    else
        dlFreeAligned(oldInst);

    void *newInst = objOBJ::dataAlloc
        ? objOBJ::dataAlloc(this, 35, num * size, nullptr)
        : dlMemalign(num * size, 0x10, "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50A);

    this->pGeom->pSharedGeom->pDataInst = newInst;
    this->pGeom->pSharedGeom->instSize = size;
    this->pGeom->pSharedGeom->instNum = num;
    this->pGeom->pSharedGeom->fvfInstanced = fvf;
    this->stateShare.state &= ~(uint64_t)0x200000; // clear OBJ_ST_SHARED_INSTANCED
}
