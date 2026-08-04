#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::Reserve @ 0x823E01F8
template<>
void dsVECTOR<m3dMATR, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
