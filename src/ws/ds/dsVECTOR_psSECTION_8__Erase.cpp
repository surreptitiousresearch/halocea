#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ps/psSECTION.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// ps subsystem section payload; only its deferred Release is called here. // boundary
struct psSECTION_DATA {
    void Release();
};

// dsVECTOR<psSECTION,8>::Erase @ 0x8251B070
// Remove `n` elements starting at `pos`. Each psSECTION is a 4-byte handle wrapping a psSECTION_DATA
// pointer; its destructor releases that payload. So the erased range is released first, then the
// tail is shifted down over the gap and the live count drops.
template<>
void dsVECTOR<psSECTION, 8>::Erase(int pos, int n)
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
        for (int eraseIndex = pos; eraseIndex < pos + n; ++eraseIndex) {
            // pData is the DB member name for this slot (handle is its legacy uint alias)
            psSECTION_DATA *payload = this->pData[eraseIndex].pData;
            if (payload)
                payload->Release();
        }
        memmove(&this->pData[pos], &this->pData[pos + n], 4 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
