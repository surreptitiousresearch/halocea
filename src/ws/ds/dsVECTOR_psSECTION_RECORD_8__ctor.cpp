#include "dsVECTOR.h"

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value)

// dsVECTOR<psSECTION_RECORD,8>::dsVECTOR(const apCL &) @ 0x82516598
// Construct an empty vector, adopting the caller's allocation call-site cookie.
// DEVIATION: the decompile shows "local variable allocation has failed"/garbage locals (v2
// used uninitialized as `this`). Disasm confirms the straightforward body: `std r4, 0xC(r3)`
// copies the 8-byte apCL argument (passed packed in a single 64-bit GPR per this ABI) into
// __cl, then pData/nElem/allocated are zeroed.
template<>
dsVECTOR<psSECTION_RECORD, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
}
