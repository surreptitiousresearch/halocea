#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::IsEmpty @ 0x82A75470
template<>
bool dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
