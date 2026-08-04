#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../scn/scnINST_CREATE_DATA.h"

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::Clear @ 0x823D7988
// Destroy every live element's scnINST_CREATE_DATA value, then reset the count. The unsigned-long
// key is trivial.
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].val.~scnINST_CREATE_DATA();
    this->nElem = 0;
}
