#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::ShrinkResize @ 0x823D8EE0
// Shrink the live count to `size`. Both pair members (int key, raw pointer value) are trivial, so
// this is just a count clamp — no per-element teardown.
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem)
        this->nElem = size;
}
