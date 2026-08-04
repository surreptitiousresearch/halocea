#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

// dsVECTOR<psSECTION,8>::operator= @ 0x8251CD98
// Copy-assign from `other` (self-assign is a no-op, detected by comparing pData). Clears the
// existing elements, conditionally reallocs, then Inserts the full source range.
// NOTE (verified against disasm): the Realloc call passes `this->nElem` (0 immediately after
// Clear()), not `other.nElem`, as the target capacity — this matches the compiled code exactly,
// so the storage is actually shrunk to empty here and regrown by the subsequent Insert's own
// doubling policy. Reproduced faithfully rather than "corrected."
template<>
dsVECTOR<psSECTION, 8> &dsVECTOR<psSECTION, 8>::operator=(const dsVECTOR<psSECTION, 8> &other)
{
    if (other.pData != this->pData) {
        this->Clear();
        if (this->allocated != other.nElem)
            this->Realloc(this->nElem);
        this->Insert(0, other.pData, other.nElem);
    }
    return *this;
}
