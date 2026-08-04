#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct objOBJ; // boundary — only referenced through LOD_INFO's object-pointer vector

// anim-subsystem per-LOD record. DB-verified (types_members animLOD::LOD_INFO): objs@0
// (dsVECTOR<objOBJ*,8>) — size 20. Modelled to match the existing ShrinkResize instantiation.
struct animLOD { struct LOD_INFO { dsVECTOR<objOBJ *, 8> objs; }; };

// dsVECTOR<animLOD::LOD_INFO,8>::Realloc @ 0x82B0D890
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live LOD
// records above the new count (freeing each record's `objs` vector), then a single dlRealloc
// resizes the storage in place — the record is trivially relocatable, so no per-element move.
// Element size 20 bytes.
template<>
void dsVECTOR<animLOD::LOD_INFO, 8>::Realloc(int size)
{
    if (size != this->allocated) {
        this->ShrinkResize(size);
        if (!IGNORE_STRONG_ASSERT && size < this->nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "size >= nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                137,
                "size", size,
                "nElem", this->nElem);
        this->pData = (animLOD::LOD_INFO *)dlRealloc(this->pData, 20 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
