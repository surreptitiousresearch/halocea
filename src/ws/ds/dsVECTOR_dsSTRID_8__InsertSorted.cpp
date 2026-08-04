#include "dsVECTOR.h"
#include "dsSTRID.h"

struct dsCMP {}; // boundary — comparator (compare inlined per-instantiation elsewhere)

// dsVECTOR<dsSTRID,8>::InsertSorted @ 0x8252E510
// No-comparator convenience overload: insert using the default dsCMP ordering policy.
template<>
int dsVECTOR<dsSTRID, 8>::InsertSorted(const dsSTRID &val, dsVECTOR<dsSTRID, 8>::INS_SORT dup)
{
    dsCMP cmp;
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
