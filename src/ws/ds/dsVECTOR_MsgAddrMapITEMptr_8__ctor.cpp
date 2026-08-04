#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::dsVECTOR(const apCL&) @ 0x82556488
// Construct an empty vector, adopting the allocation call-site cookie `cl` (copied by value into
// __cl). DEVIATION: the raw decompile shows `this->__cl.file = (const char*)this` and
// `this->__cl.line = (int)cl` — a register-allocation artifact (the disassembly's `std r4,
// arg_18(r1)` followed by a plain 8-byte reload/store of arg_18 into this+0xC/this+0x10 confirms
// this is really a straight `*cl` -> `this->__cl` copy, not a self-referential store).
template<>
dsVECTOR<dsMsgAddrMapITEM *, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = 0;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl.file = cl.file;
    this->__cl.line = cl.line;
}
