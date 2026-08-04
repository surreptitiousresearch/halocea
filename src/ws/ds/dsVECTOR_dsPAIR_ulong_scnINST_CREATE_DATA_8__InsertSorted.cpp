#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../scn/scnINST_CREATE_DATA.h"

struct dsCMP {}; // boundary — stateless comparator forwarded to the templated InsertSorted<dsCMP>

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::InsertSorted @ 0x823E5000
// Non-templated overload: builds a default (zero-initialized) dsCMP and forwards to the templated
// InsertSorted<dsCMP> (an extern-boundary sibling call, not re-reversed here).
template<>
int dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::InsertSorted(
        const dsPAIR<unsigned long, scnINST_CREATE_DATA> &val, INS_SORT dup)
{
    dsCMP cmp = {};
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
