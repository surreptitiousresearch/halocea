#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Clear @ 0x823CF748
// Destroy every live element and reset the count. dsSTRID is trivially destructible (a single
// interned const char*), so the per-element destructor loop is empty and collapses to nElem=0.
template<>
void dsVECTOR<dsSTRID, 8>::Clear()
{
    this->nElem = 0;
}
