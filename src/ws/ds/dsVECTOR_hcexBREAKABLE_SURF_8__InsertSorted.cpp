#include "dsVECTOR.h"

struct hcexBREAKABLE_SURF; // boundary
struct dsCMP {}; // boundary — stateless three-way comparator (see FindSorted)

// dsVECTOR<hcexBREAKABLE_SURF,8>::InsertSorted @ 0x823B28D8
// No-comparator convenience overload: insert using the default dsCMP (bsp, idx) ordering policy.
template<>
int dsVECTOR<hcexBREAKABLE_SURF, 8>::InsertSorted(
        const hcexBREAKABLE_SURF &val, dsVECTOR<hcexBREAKABLE_SURF, 8>::INS_SORT dup)
{
    dsCMP cmp;
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
