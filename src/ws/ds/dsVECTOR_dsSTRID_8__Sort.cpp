#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — comparator (compare inlined per-instantiation elsewhere)

// dsVECTOR<dsSTRID,8>::Sort @ 0x82C48FC8
// No-arg convenience overload: sort using the default comparator/swap policies.
template<>
void dsVECTOR<dsSTRID, 8>::Sort()
{
    dsCMP cmp;
    dsSWAP<dsSTRID> swap;
    this->Sort<dsCMP, dsSWAP<dsSTRID>>(cmp, swap);
}
