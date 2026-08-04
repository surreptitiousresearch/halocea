#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::Length @ 0x82A95CE8
template<>
int dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::Length() const
{
    return this->nElem;
}
