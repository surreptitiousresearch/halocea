#include "dsVECTOR.h"

struct hcexBREAKABLE_SURF; // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::Length @ 0x823B0978
// Number of live elements.
template<>
int dsVECTOR<hcexBREAKABLE_SURF, 8>::Length() const
{
    return this->nElem;
}
