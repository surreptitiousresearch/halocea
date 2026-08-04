#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Length @ 0x82A75480
template<>
int dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Length() const
{
    return this->nElem;
}
