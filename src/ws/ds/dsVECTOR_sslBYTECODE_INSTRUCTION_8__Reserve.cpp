#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::Reserve @ 0x83089DF8
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
