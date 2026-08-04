#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// boundary — objOBJ (Blam/ws object); only referenced through the LOD's object-pointer vector.
struct objOBJ;

// anim-subsystem per-LOD record. DB-verified (types_members animLOD::LOD_INFO): objs@0
// (dsVECTOR<objOBJ*,8>) — size 20. Modelled inline to match the sibling instantiations.
struct animLOD { struct LOD_INFO { dsVECTOR<objOBJ *, 8> objs; }; };

// dsVECTOR<animLOD::LOD_INFO,8>::~dsVECTOR @ 0x82B0D0B8
// Destroy every live LOD record (each record's inlined objs vector frees its own storage),
// then release the vector's backing store. The records are trivially relocatable.
template<>
dsVECTOR<animLOD::LOD_INFO, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        dlFree(this->pData[i].objs.pData); // inlined ~LOD_INFO -> ~dsVECTOR<objOBJ*,8>
    dlFree(this->pData);
}
