#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"
#include <new> // placement new

extern const char empty_string[]; // boundary — shared empty-string global (also used by ds_assert_boundary)

// dsVECTOR<apCOUNTER_UNIT,8>::CallNew<apCOUNTER_UNIT> @ 0x82719320
// Placement-construct helper: builds an empty-named counter into the raw slot `t`.
template<>
template<>
apCOUNTER_UNIT *dsVECTOR<apCOUNTER_UNIT, 8>::CallNew<apCOUNTER_UNIT>(apCOUNTER_UNIT *t)
{
    if (t)
        new (t) apCOUNTER_UNIT(empty_string);
    return t;
}
