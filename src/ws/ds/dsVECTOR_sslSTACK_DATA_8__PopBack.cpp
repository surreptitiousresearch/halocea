#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

// dsVECTOR<sslSTACK_DATA,8>::PopBack @ 0x82AF1340
template<>
void dsVECTOR<sslSTACK_DATA, 8>::PopBack()
{
    int nElem = this->nElem;
    if (nElem)
        this->ShrinkResize(nElem - 1);
}
