#include "mtlTBL.h"

// mtlTBL::Clear @ 0x82C2CCF8
// Destroy every live material slot, shrink the backing store to match the (now zero)
// live count, and restore the default texel-density state.
void mtlTBL::Clear()
{
    this->mtlList.Clear();
    this->mtlList.Realloc(this->mtlList.nElem);
    this->Init();
}
