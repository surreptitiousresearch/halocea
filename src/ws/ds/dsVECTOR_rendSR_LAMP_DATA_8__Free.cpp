#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Free @ 0x829ECB00
// Release all backing storage (reallocate down to zero capacity).
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Free()
{
    this->Realloc(0);
}
