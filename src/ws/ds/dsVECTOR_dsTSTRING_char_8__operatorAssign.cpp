#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::operator= @ 0x823DC308
// Deviation/CAVEAT: verified against disasm — after Clear() (which zeroes nElem but not
// allocated), the reallocation-size check reads `this->allocated != src.nElem` but the ensuing
// Realloc call passes `this->nElem` (now 0, post-Clear), not `src.nElem`. This looks like a latent
// quirk in the original compiled code (also present, byte-identical, in the dsPARAM instantiation
// of this same operator=), reproduced faithfully rather than "corrected": it is harmless because
// the subsequent Insert(0, src.pData, src.nElem) grows storage back via its own doubling policy.
template<>
dsVECTOR<dsTSTRING<char>, 8> &dsVECTOR<dsTSTRING<char>, 8>::operator=(const dsVECTOR<dsTSTRING<char>, 8> &src)
{
    if (src.pData != this->pData) {
        this->Clear();
        if (this->allocated != src.nElem)
            this->Realloc(this->nElem);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
