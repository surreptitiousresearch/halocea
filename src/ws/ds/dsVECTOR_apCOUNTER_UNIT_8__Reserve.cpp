#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"

// dsVECTOR<apCOUNTER_UNIT,8>::Reserve @ 0x82718D60
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
