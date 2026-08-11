#include "objOBJ.h"
#include "OBJ_DATA.h"
#include "obj_boundary.h"
#include "../ds/ds_assert_boundary.h"
// 0x82A7E6E0  ?FreeObjData@objOBJ@@QAAXHPAX@Z
// Release a per-object data block of class `data`. Routes through the pluggable dataFree hook
// when installed; otherwise the aligned data class (35) uses dlFreeAligned and the rest dlFree.

// Static deleter hook storage (?dataFree@objOBJ@@2P6AXPAV1@PAXH@ZA, 0x842264C4).
objOBJ::DataFreeProc objOBJ::dataFree = nullptr;

void objOBJ::FreeObjData(unsigned int data, void *oldData)
{
    if (!IGNORE_STRONG_ASSERT && data >= OBJ_DATA_MAX)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "data >= 0 && data < OBJ_DATA_MAX",
            "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 1330,
            empty_string);

    if (objOBJ::dataFree)
        objOBJ::dataFree(this, oldData, data);
    else if (data == OBJ_DATA_INST)
        dlFreeAligned(oldData);
    else
        dlFree(oldData);
}
