#include "dsVECTOR.h"

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value)

// dsVECTOR<psSECTION_RECORD,8>::Compact @ 0x82519230
// Shrink the backing store to exactly fit the live elements.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Compact()
{
    this->Realloc(this->nElem);
}
