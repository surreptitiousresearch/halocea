#include "objOBJ.h"
// 0x82A7EDD8  ?_FreeGeom@objOBJ@@QAAXXZ
// Release the unshared geometry; the shared payload is deleted too unless stateShare bit 0x10
// marks it shared with another object.
// NOTE: corrected the decompiler's HIDWORD(state)+12 packing artifact — the disassembly deletes
// pGeom->pSharedGeom (offset 12 of pGeom), not an offset off the state value.

void objOBJ::_FreeGeom()
{
    if (pGeom)
    {
        if ((stateShare.state & 0x10) == 0)
            operator delete(pGeom->pSharedGeom);
        operator delete(pGeom);
        pGeom = nullptr;
    }
}
