#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Compact @ 0x823D9108
// Shrink the backing store to exactly fit the live elements.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Compact()
{
    this->Realloc(this->nElem);
}
