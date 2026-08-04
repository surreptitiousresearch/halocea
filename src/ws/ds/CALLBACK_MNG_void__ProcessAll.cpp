#include "CALLBACK_MNG.h"
#include "ds_assert_boundary.h"

// ds::CALLBACK_MNG<void (__cdecl*)(void)>::ProcessAll @ 0x823DF6D8
// Invoke every registered callback in order. The live count is re-read on each iteration (a
// callback may register or unregister others), and the loop terminates once the running index
// reaches the current element count. Each access is bounds-checked via the strong-assert helper.
template<>
void ds::CALLBACK_MNG<void (__cdecl *)(void)>::ProcessAll()
{
    int nElem = this->callbacks.nElem;
    int *pnElem = &this->callbacks.nElem;
    int idx = 0;
    if (nElem > 0) {
        int slot = 0;
        do {
            int checkIdx = idx;
            if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= nElem))
                STRONG_ASSERT2_HELPER::asserd<int, int>(
                    "idx >= 0 && idx < nElem",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                    567,
                    "idx", checkIdx,
                    "nElem", *pnElem);
            this->callbacks.pData[slot]();
            nElem = *pnElem;
            ++idx;
            ++slot;
        } while (idx < *pnElem);
    }
}
