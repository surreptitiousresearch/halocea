#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::dsVECTOR(const apCL&) @ 0x823C1B60
// Construct an empty vector, adopting the caller's allocation call-site cookie verbatim (disasm:
// a raw 8-byte copy of {file,line} into __cl).
template<>
dsVECTOR<dsDATA, 8>::dsVECTOR(const apCL &cl)
    : pData(nullptr), nElem(0), allocated(0), __cl(cl)
{
}
