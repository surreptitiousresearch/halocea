#include "dsVECTOR.h"
#include "dsDATA.h"
#include "dsSWAP.h"
#include "dsREF_VECTOR.h"

// dsVECTOR<dsDATA,8>::Sort<dsREF_VECTOR::SortCmp> @ 0x8261C078
// Convenience one-template-arg overload: default-constructs a stateless dsSWAP<dsDATA> and
// delegates to the (cmp, swap) form.
template<>
template<>
void dsVECTOR<dsDATA, 8>::Sort<dsREF_VECTOR::SortCmp>(const dsREF_VECTOR::SortCmp &cmp)
{
    dsSWAP<dsDATA> swap;
    this->Sort(cmp, swap);
}
