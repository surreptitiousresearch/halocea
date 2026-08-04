#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "../ds/ds_assert_boundary.h"
// 0x82A7F638  ?_AllocInterleavedData@objOBJ@@QAAXHE_K@Z
// (Re)allocate the interleaved vertex buffer (`size` bytes/vertex over `nVert` vertices),
// install it plus its FVF via SetInterleavedData, then clear the "interleaved data shared"
// state bit (0x400000).
//
// CAVEAT: mangle is (H, E, _K) = (int nVert, unsigned char size, unsigned __int64 fvf). The
// decompiler fabricated a phantom int arg and lost `fvf` (passed uninitialised to
// SetInterleavedData); restored per the mangle and the disasm.

void objOBJ::_AllocInterleavedData(int nVert, unsigned char size, unsigned __int64 fvf)
{
    if (!this->pGeom)
        return;

    if (!IGNORE_STRONG_ASSERT && (this->stateShare.state & 0x400000) != 0)
        STRONG_ASSERT_DUMMY().Crash(
            "!stateShare.Is(OBJ_ST_SHARED_INTERLEAVED_DATA)",
            "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 1435,
            dsStrongAssertMessage);

    void *interleavedData =
        AllocObjData(36, size * nVert, this->pGeom->pSharedGeom->pDataInterleaved);
    SetInterleavedData(interleavedData, size, fvf);
    this->stateShare.state &= ~0x400000ll; // clear OBJ_ST_SHARED_INTERLEAVED_DATA
}
