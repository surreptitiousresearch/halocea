#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::PopBack @ 0x82804690
// Drop the last live element (no-op on an empty vector).
template<>
void dsVECTOR<dsSTRID, 8>::PopBack()
{
    if (this->nElem)
        this->ShrinkResize(this->nElem - 1);
}
