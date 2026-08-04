#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::Length @ 0x82AD7AC8
// Number of live elements.
template<>
int dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::Length() const
{
    return this->nElem;
}
