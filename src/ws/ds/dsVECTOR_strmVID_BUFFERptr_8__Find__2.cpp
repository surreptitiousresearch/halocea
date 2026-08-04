#include "dsVECTOR.h"
#include "dsCMP.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Find<strmVID_BUFFER*> @ 0x82698680
// No-comparator overload: delegates to the dsCMP-based Find with startPos 0.
template<>
template<>
int dsVECTOR<strmVID_BUFFER *, 8>::Find<strmVID_BUFFER *>(strmVID_BUFFER *const &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, strmVID_BUFFER *>(key, cmp, 0);
}
