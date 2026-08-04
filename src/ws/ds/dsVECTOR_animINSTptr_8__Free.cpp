#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Free @ 0x8318D218
// Release the backing storage entirely (Realloc to zero capacity).
template<>
void dsVECTOR<animINST *, 8>::Free()
{
    this->Realloc(0);
}
