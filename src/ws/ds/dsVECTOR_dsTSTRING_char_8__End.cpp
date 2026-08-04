#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::End @ 0x82632E78 (const overload)
template<>
const dsTSTRING<char> *dsVECTOR<dsTSTRING<char>, 8>::End() const
{
    return &this->pData[this->nElem];
}
