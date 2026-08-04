#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"

// dsVECTOR<vidOBJ_SPLIT,8>::Length @ 0x8273EB10
// Number of live elements.
template<>
int dsVECTOR<vidOBJ_SPLIT, 8>::Length() const
{
    return this->nElem;
}
