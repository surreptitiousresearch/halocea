#include "dsVECTOR.h"
#include "dsPARAM.h"

// dsVECTOR<dsPARAM,8>::Compact @ 0x827F1C10
template<>
void dsVECTOR<dsPARAM, 8>::Compact()
{
    this->Realloc(this->nElem);
}
