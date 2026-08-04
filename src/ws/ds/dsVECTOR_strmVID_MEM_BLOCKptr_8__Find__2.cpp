#include "dsVECTOR.h"
#include "dsCMP.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::Find<strmVID_MEM_BLOCK*> @ 0x826986B8
// No-comparator overload: delegates to the dsCMP-based Find with startPos 0.
template<>
template<>
int dsVECTOR<strmVID_MEM_BLOCK *, 8>::Find<strmVID_MEM_BLOCK *>(strmVID_MEM_BLOCK *const &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, strmVID_MEM_BLOCK *>(key, cmp, 0);
}
