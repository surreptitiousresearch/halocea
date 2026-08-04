#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include <new>

// dsVECTOR<psSECTION_RECORD,8>::CallNew<psSECTION_RECORD> @ 0x82517690
// Placement-"construct" helper: default-constructs the fields of a raw slot `t` in place
// (interns the empty-string key id, clears the stored value's type) and returns it. A null `t`
// is a no-op passthrough.
template<>
template<>
psSECTION_RECORD *dsVECTOR<psSECTION_RECORD, 8>::CallNew<psSECTION_RECORD>(psSECTION_RECORD *t)
{
    if (!t)
        return nullptr;
    new (&t->keyStrId) dsSTRID();
    t->value.type = nullptr;
    return t;
}
