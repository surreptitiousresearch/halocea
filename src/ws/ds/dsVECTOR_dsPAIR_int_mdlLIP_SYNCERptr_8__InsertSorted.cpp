#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)
struct dsCMP {};      // boundary — stateless comparator forwarded to the templated InsertSorted<dsCMP>

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::InsertSorted @ 0x823E5070
// Non-templated overload: builds a default (zero-initialized) dsCMP and forwards to the templated
// InsertSorted<dsCMP> (an extern-boundary sibling call, not re-reversed here).
template<>
int dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::InsertSorted(
        const dsPAIR<int, mdlLIP_SYNCER *> &val, INS_SORT dup)
{
    dsCMP cmp = {};
    return this->InsertSorted<dsCMP>(val, cmp, dup);
}
