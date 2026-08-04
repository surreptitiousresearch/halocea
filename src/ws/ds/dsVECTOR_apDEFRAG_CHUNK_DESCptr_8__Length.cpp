#include "dsVECTOR.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::Length @ 0x827B25F8
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::Length() const
{
    return this->nElem;
}
