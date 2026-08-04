#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Reserve @ 0x82AC5478
template<>
void dsVECTOR<msgMSG, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
