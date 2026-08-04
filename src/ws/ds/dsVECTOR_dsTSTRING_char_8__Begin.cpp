#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Begin @ 0x82632E68 (const overload)
template<>
const dsTSTRING<char> *dsVECTOR<dsTSTRING<char>, 8>::Begin() const
{
    return this->pData;
}
