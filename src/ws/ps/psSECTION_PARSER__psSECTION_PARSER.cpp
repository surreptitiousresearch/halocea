#include "psSECTION_PARSER.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// 0x8251BFB4 — default-construct a parser: empty error text, empty deferred parent-setup list.
// The `read` reader state is left zero-initialized by the caller (Parse() sets it up explicitly).

psSECTION_PARSER::psSECTION_PARSER()
{
    errorDesc.pBuffer = nullptr;
    errorDesc.UnsafeInitEmpty();
    parentsSetup.pData = nullptr;
    parentsSetup.nElem = 0;
    parentsSetup.allocated = 0;
}
