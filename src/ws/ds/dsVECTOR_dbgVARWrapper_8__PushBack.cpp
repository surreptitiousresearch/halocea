#include "dsVECTOR.h"
#include "../wb/dbgVARWrapper.h"
#include <new>

// dsVECTOR<wb::dbgVARWrapper,8>::PushBack @ 0x8269E4C8
// Append a copy of `val` and return a reference to the new last element. dbgVARWrapper's `name` is a
// dsSTRID (copy-constructed by interning), `dbgVar` is cleared to null and `defaultVal` copied. When
// the vector must grow, the strid is first interned into a temporary (it may alias the backing store)
// and released afterwards; here the temporary's construct/destruct bracket the Realloc.
template<>
wb::dbgVARWrapper &dsVECTOR<wb::dbgVARWrapper, 8>::PushBack(const wb::dbgVARWrapper &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        wb::dbgVARWrapper *slot = &this->pData[nElem];
        if (slot) {
            new (&slot->name) dsSTRID(val.name.id, 0); // dsSTRID copy-intern
            slot->dbgVar = nullptr;
            slot->defaultVal = val.defaultVal;
        }
    } else {
        dsSTRID tempName(val.name.id, 0);
        bool tempDefault = val.defaultVal;
        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);
        wb::dbgVARWrapper *slot = &this->pData[this->nElem];
        if (slot) {
            new (&slot->name) dsSTRID(tempName.id, 0);
            slot->dbgVar = nullptr;
            slot->defaultVal = tempDefault;
        }
        // tempName destroyed here (dsSTRID dtor is trivial for the interned id).
    }
    ++this->nElem;
    return this->Back();
}
