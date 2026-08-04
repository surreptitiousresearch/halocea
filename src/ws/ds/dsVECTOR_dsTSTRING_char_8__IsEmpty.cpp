#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::IsEmpty @ 0x823CF358
template<>
bool dsVECTOR<dsTSTRING<char>, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
