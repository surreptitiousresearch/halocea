#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::Length @ 0x823B2DC8
template<>
int dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::Length() const
{
    return this->nElem;
}
