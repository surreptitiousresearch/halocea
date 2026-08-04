#include "dsVECTOR.h"
#include "../ssl/sslFOREACH_DATA.h"

// dsVECTOR<sslFOREACH_DATA,8>::Clear @ 0x82AF0590
// Destroy every live element's checkClass ref (releasing the referenced script class) and reset
// the count. lastObjIdx is a plain int (nothing to destroy).
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].checkClass.~sslCLASS_REF();
    this->nElem = 0;
}
