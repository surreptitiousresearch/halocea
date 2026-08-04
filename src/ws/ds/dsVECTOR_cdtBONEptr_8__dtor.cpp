#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<cdtBONE*,8>::~dsVECTOR @ 0x828B90B8
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<cdtBONE *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
