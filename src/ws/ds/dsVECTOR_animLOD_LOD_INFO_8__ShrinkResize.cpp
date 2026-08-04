#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// boundary — objOBJ (Blam/ws object); only referenced through the LOD's object-pointer vector.
struct objOBJ;

// anim-subsystem per-LOD record. DB-verified (types_members animLOD::LOD_INFO): objs@0
// (dsVECTOR<objOBJ*,8>) — size 20. Its inlined ~LOD_INFO frees the `objs` vector storage;
// the element is a raw pointer, so no per-element destruction is required.
struct animLOD { struct LOD_INFO { dsVECTOR<objOBJ *, 8> objs; }; };

// dsVECTOR<animLOD::LOD_INFO,8>::ShrinkResize @ 0x82B0D298
// Shrink the live count to `size`, destroying every LOD record from `size` up to the old count.
template<>
void dsVECTOR<animLOD::LOD_INFO, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i)
            dlFree(this->pData[i].objs.pData); // inlined ~LOD_INFO -> ~dsVECTOR<objOBJ*,8>
        this->nElem = size;
    }
}
