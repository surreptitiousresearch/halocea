#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::dsVECTOR(const apCL&) @ 0x82556118
// Construct an empty vector, adopting the allocation call-site cookie `cl` (copied by value into
// __cl). DEVIATION: same register-allocation artifact as the sibling MAP-vector ctors — the raw
// decompile's `this->__cl.file = (const char*)this` / `.line = (int)cl` is corrected to a plain
// field-pair copy from `*cl`.
template<>
dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = 0;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl.file = cl.file;
    this->__cl.line = cl.line;
}
