// FUNCTION_INDEX entry: dsVECTOR<apCOUNTER_UNIT,8>::CallNew<apCOUNTER_UNIT> @0x82719320 (??$CallNew@UapCOUNTER_UNIT@@@?$dsVECTOR@UapCOUNTER_UNIT@@$07@@QAAPAUapCOUNTER_UNIT@@PAU1@@Z)
#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"
#include <new> // placement new

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

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
