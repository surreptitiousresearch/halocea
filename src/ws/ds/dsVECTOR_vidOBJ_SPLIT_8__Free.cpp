#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"

// dsVECTOR<vidOBJ_SPLIT,8>::Free @ 0x82C875E8
// Release all backing storage (reallocate down to zero capacity).
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::Free()
{
    this->Realloc(0);
}
