#include "dsVECTOR.h"
#include "../prop/propBASE.h"
#include "../ds/dsSTRID.h"

// Stateless name/id comparator functor: extracts propBASE's name (via propBASE::GetName) and
// compares it against the NUL-terminated key string. Layout-less; the compare is inlined below.
template<class T, int A, int B>
struct dsNAMEID_CMP; // boundary

// dsVECTOR<propBASE*,8>::Find<dsNAMEID_CMP<propBASE,0,0>,char const*> @ 0x826C3728
// Linear scan from startPos for the first prop whose name (propBASE::GetName) equals `key`
// (pointer-value compare of the interned dsSTRID char* id — not a string compare); returns its
// index, or -1 if none found.
template<>
template<>
int dsVECTOR<propBASE *, 8>::Find<dsNAMEID_CMP<propBASE, 0, 0>, char const *>(
    char const *const &key, const dsNAMEID_CMP<propBASE, 0, 0> & /*cmp*/, int startPos) const
{
    if (startPos >= this->nElem)
        return -1;

    int scanIndex = startPos;
    while (scanIndex != this->nElem) {
        dsSTRID name = this->pData[scanIndex]->GetName();
        const char *nameId = name.CStr();
        // pointer-value three-way compare against the interned key id (dsSTRID ids are pooled,
        // so equality is a pointer compare, not a string compare)
        int order = (nameId >= key) ? (key < nameId) : -1;
        if (!order)
            return scanIndex;
        ++scanIndex;
    }
    return -1;
}
