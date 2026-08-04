#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Begin @ 0x82BDCCF8 (non-const overload)
template<>
dsTSTRING<char> *dsVECTOR<dsTSTRING<char>, 8>::Begin()
{
    return this->pData;
}
