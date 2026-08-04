#include "dsVECTOR.h"
#include "dsCMP.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Find<animINST*> @ 0x8287A540
// No-comparator overload: delegates to the dsCMP-based Find with startPos 0.
template<>
template<>
int dsVECTOR<animINST *, 8>::Find<animINST *>(animINST *const &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, animINST *>(key, cmp, 0);
}
