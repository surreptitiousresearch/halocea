#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Reserve @ 0x823EE230
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<animINST *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
