#include "dsVECTOR.h"

struct apDEFRAG_POOL; // boundary — ws-engine ap: defragmentable memory pool (element is a pointer)

// dsVECTOR<apDEFRAG_POOL*,8>::Length @ 0x827B2600
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<apDEFRAG_POOL *, 8>::Length() const
{
    return this->nElem;
}
