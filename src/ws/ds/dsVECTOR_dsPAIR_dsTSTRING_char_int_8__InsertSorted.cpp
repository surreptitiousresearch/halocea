#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless comparator forwarded to the templated InsertSorted<dsCMP>

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::InsertSorted @ 0x823B8F68
// Non-templated overload: builds a default (zero-initialized) dsCMP and forwards to the templated
// InsertSorted<dsCMP> (an extern-boundary sibling call, not re-reversed here).
template<>
int dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::InsertSorted(
        const dsPAIR<dsTSTRING<char>, int> &val, INS_SORT dup)
{
    dsCMP cmp = {};
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
