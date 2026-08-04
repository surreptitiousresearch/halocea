#include "objOBJ.h"
#include "OBJ_DATA.h"
#include "../ds/ds_assert_boundary.h"
// 0x82A7E640  ?AllocObjData@objOBJ@@QAAPAXHHPAX@Z
// Allocate (or reallocate) a per-object data block of class `data`. Routes through the pluggable
// dataAlloc hook when installed, otherwise the default StdAllocObjData allocator.

// Static allocator hook storage (?dataAlloc@objOBJ@@2P6APAXPAV1@HHPAX@ZA, 0x842264C0).
objOBJ::DataAllocProc objOBJ::dataAlloc = nullptr;

void *objOBJ::AllocObjData(unsigned int data, int size, void *oldData)
{
    if (!IGNORE_STRONG_ASSERT && data >= OBJ_DATA_MAX)
        STRONG_ASSERT_DUMMY().Crash(
            "data >= 0 && data < OBJ_DATA_MAX",
            "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 1316,
            dsStrongAssertMessage);

    if (objOBJ::dataAlloc)
        return objOBJ::dataAlloc(this, data, size, oldData);
    return objOBJ::StdAllocObjData(this, data, size, oldData);
}
