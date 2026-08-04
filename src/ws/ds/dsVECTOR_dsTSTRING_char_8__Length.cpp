#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Length @ 0x823B2DA8
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Length() const
{
    return this->nElem;
}
