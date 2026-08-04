#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::End @ 0x82BDCD00 (non-const overload)
template<>
dsTSTRING<char> *dsVECTOR<dsTSTRING<char>, 8>::End()
{
    return &this->pData[this->nElem];
}
