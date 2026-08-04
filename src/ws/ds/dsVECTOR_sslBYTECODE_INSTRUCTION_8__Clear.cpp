#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::Clear @ 0x83087CD0
// Destroy every live element and reset the count. sslBYTECODE_INSTRUCTION is trivially
// destructible (a POD opcode+union), so the per-element destructor loop is empty and collapses
// to nElem=0.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::Clear()
{
    this->nElem = 0;
}
