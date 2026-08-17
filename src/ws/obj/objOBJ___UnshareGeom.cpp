/* ?_UnshareGeom@objOBJ@@QAAXXZ @0x82A7F798 */
#include <new>
#include "objOBJ.h"
// 0x82A7F798  ?_UnshareGeom@objOBJ@@QAAXXZ
// If this object currently shares its geometry payload (stateShare bit 0x10), give it a
// private copy: clear the shared flag and replace pSharedGeom with a fresh copy-constructed
// objGEOM_SHARED. Idempotent — a no-op once the object owns its geometry.
//
// CAVEAT: the decompiler exposed the standard "p = new X(...)" idiom as two aliased temps
// (v7 from operator new, v8 the constructed result) then stored the UNINITIALISED alias; the
// real code stores the single allocated pointer after copy-constructing it. Reconstructed with
// the engine's debug placement-new (size, file, line). The phantom trailing register args
// (a2..a6) on the decompiled prototype are ABI artifacts — the mangle (XZ) confirms none.

// Debug tagging allocator used engine-wide: operator new(size, __FILE__, __LINE__).
extern void *operator new(size_t size, const char *file, unsigned int line);

void objOBJ::_UnshareGeom()
{
    if ((stateShare.state & 0x10) == 0)
        return;

    stateShare.state &= ~0x10; // clear the "geometry shared" flag

    pGeom->pSharedGeom = new ("D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x137)
        objGEOM_SHARED(*pGeom->pSharedGeom); // copy-construct a private geometry payload
}
