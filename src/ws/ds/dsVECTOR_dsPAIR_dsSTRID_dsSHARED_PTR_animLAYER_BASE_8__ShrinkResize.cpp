#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSHARED_PTR.h"
#include "Deleter.h"
#include "../anim/animLAYER_BASE.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPAIR<dsSTRID,dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE>>>,8>::ShrinkResize
// @ 0x8267E808
// Shrink the live count to `size`, releasing each element's shared-pointer value above the new
// count (dropping the shared count and invoking the deleter at zero). The dsSTRID key needs no
// teardown.
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index)
            this->pData[index].val.Reset();
        this->nElem = size;
    }
}
