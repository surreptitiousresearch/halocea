#include "dsVECTOR.h"
#include "dsPARAM.h"

// dsVECTOR<dsPARAM,8>::Length @ 0x823CF388
template<>
int dsVECTOR<dsPARAM, 8>::Length() const
{
    return this->nElem;
}
