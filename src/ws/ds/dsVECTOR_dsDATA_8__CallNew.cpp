#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::CallNew<dsDATA> @ 0x82570420
// Placement-"construct" helper: null-initialise the type-erased slot `t` in place (an empty
// dsDATA) and return it (null-t is a no-op passthrough).
template<>
template<>
dsDATA *dsVECTOR<dsDATA, 8>::CallNew<dsDATA>(dsDATA *t)
{
    if (!t)
        return nullptr;
    t->type = nullptr;
    return t;
}
