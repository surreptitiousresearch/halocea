#include "dsVECTOR.h"
#include "../inp/inpDEVICE.h"
#include "../ds/dsSTRID.h"

// Stateless comparator functor: extracts inpDEVICE::type (a dsSTRID) and compares its interned
// id against the key's id. Layout-less; the compare is inlined below. (inp subsystem.)
struct CMP; // boundary

// dsVECTOR<inpDEVICE*,8>::Find<CMP,dsSTRID> @ 0x8263FA00
// Linear scan from startPos for the first device whose type id (inpDEVICE::type.id) equals
// `key.id` (interned-pointer compare, not a string compare); returns its index, or -1 if none.
template<>
template<>
int dsVECTOR<inpDEVICE *, 8>::Find<CMP, dsSTRID>(
    const dsSTRID &key, const CMP & /*cmp*/, int startPos) const
{
    if (startPos >= this->nElem)
        return -1;

    const char *keyId = key.id;
    int scanIndex = startPos;
    while (true) {
        const char *elemId = this->pData[scanIndex]->type.id;
        if (elemId >= keyId && keyId >= elemId) // equality via two unsigned/pointer compares
            break;
        if (++scanIndex == this->nElem)
            return -1;
    }
    return scanIndex;
}
