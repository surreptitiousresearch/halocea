#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::CallNew<dsTSTRING<char>> @ 0x82686960
// Placement-"construct" the slot at `t`: default-construct a dsTSTRING<char> in place by adopting
// the process-wide shared empty-string singleton (lazily allocated on first use), matching
// New<dsTSTRING<char>> @ 0x82685BC8 which this tail-calls. A null `t` is a no-op passthrough.
template<>
template<>
dsTSTRING<char> *dsVECTOR<dsTSTRING<char>, 8>::CallNew<dsTSTRING<char>>(dsTSTRING<char> *t)
{
    if (!t)
        return nullptr;
    t->pBuffer = nullptr;
    t->UnsafeInitEmpty();
    return t;
}
