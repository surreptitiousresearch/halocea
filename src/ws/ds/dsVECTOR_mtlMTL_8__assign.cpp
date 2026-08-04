#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::operator= @ 0x82689E40
// Copy-assign every element from `other`: clear this vector, resize storage to this vector's own
// (now-zero) live count if capacity doesn't already match `other`'s count, then splice in the
// full source range. Self-assignment (same backing storage) is a no-op.
//
// DEVIATION NOTE: after Clear() this->nElem is 0, so the Realloc call below passes this->nElem
// (0 at that point), not other.nElem — verified against the disassembly
// (`lwz r4, 4(r31)` loads `this`'s own nElem, not `other`'s), matching the decompiler exactly.
template<>
dsVECTOR<mtlMTL, 8> &dsVECTOR<mtlMTL, 8>::operator=(const dsVECTOR<mtlMTL, 8> &other)
{
    if (other.pData != this->pData) {
        this->Clear();
        if (this->allocated != other.nElem)
            this->Realloc(this->nElem);
        this->Insert(0, other.pData, other.nElem);
    }
    return *this;
}
