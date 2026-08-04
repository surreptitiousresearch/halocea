#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending comparator (matches ds_assert_boundary siblings)

// dsVECTOR<dsTSTRING<char>,8>::InsertSorted (no explicit comparator) @ 0x823E2010
// Convenience overload: default-constructs a stateless dsCMP and delegates to InsertSorted<dsCMP>.
template<>
int dsVECTOR<dsTSTRING<char>, 8>::InsertSorted(const dsTSTRING<char> &val, INS_SORT dup)
{
    dsCMP cmp;
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
