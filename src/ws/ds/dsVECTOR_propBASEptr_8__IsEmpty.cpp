#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

// dsVECTOR<propBASE*,8>::IsEmpty @ 0x823CF3B0
// True when the vector holds no live elements.
template<>
bool dsVECTOR<propBASE *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
