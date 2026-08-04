#include "dsSTRID.h"

// dsSTRID::dsSTRID(const dsSTRID&, bool) @ 0x825280F0
// Copy `other`'s already-interned id verbatim; no re-interning is performed. The trailing bool
// has no observed effect in the body -- it exists purely to disambiguate this constructor from
// the implicit compiler-generated copy constructor.
dsSTRID::dsSTRID(const dsSTRID &other, bool)
{
    this->id = other.id;
}
