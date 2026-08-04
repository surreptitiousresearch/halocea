#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::Compact @ 0x8308A078
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::Compact()
{
    this->Realloc(this->nElem);
}
