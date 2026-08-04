#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::Erase @ 0x823DACD8
// Remove `n` elements starting at `pos` by shifting the tail down over them and dropping the live
// count. Elements are {int,pointer} pairs (8 bytes) and trivially relocatable, so no per-element
// destructor runs.
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos,
            "nElem", this->nElem,
            "n", n);

    if (n) {
        memmove(&this->pData[pos], &this->pData[pos + n], 8 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
