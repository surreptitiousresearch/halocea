#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../anim/animLOD_LOD_INFO.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<animLOD::LOD_INFO,8>::Resize @ 0x82B0E628
// Resize the live count to `size`. Growing past capacity reallocates, then default-constructs the
// newly exposed LOD_INFO records (empty `objs` vector). Shrinking destroys each dropped record,
// which reduces to freeing its `objs` backing storage.
template<>
void dsVECTOR<animLOD::LOD_INFO, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index) {
                animLOD::LOD_INFO *lod = &this->pData[index];
                lod->objs.pData = 0;
                lod->objs.nElem = 0;
                lod->objs.allocated = 0;
                lod->objs.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\lod.h";
                lod->objs.__cl.line = 65;
            }
        }
        this->nElem = size;
    } else {
        for (int index = size; index < this->nElem; ++index)
            dlFree(this->pData[index].objs.pData);
        this->nElem = size;
    }
}
