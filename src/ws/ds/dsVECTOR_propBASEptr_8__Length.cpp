#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

// dsVECTOR<propBASE*,8>::Length @ 0x823CFA50
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<propBASE *, 8>::Length() const
{
    return this->nElem;
}
