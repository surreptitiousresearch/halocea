#include "dsVECTOR.h"
#include "../msg/saLIST.h"

// dsVECTOR<saLIST,8>::Clear @ 0x8292B170
// Destroy every live element's addList (freeing every cell) and reset the count. `msg` is a
// plain int (nothing to destroy).
template<>
void dsVECTOR<saLIST, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].addList.Clear();
    this->nElem = 0;
}
