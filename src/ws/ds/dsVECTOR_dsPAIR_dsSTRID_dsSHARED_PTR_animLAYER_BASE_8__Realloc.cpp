#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSHARED_PTR.h"
#include "Deleter.h"
#include "../anim/animLAYER_BASE.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<dsSTRID,dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE>>>,8>::Realloc
// @ 0x8267E900
// Resize the backing store to hold exactly `size` elements. ShrinkResize first releases any
// elements above the new count, then a single dlRealloc resizes the storage in place.
// Element size 12 bytes.
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8>::Realloc(int size)
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
        this->pData = (dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > > *)
            dlRealloc(this->pData, 12 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
