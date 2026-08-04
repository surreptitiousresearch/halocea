#include "dsVECTOR.h"

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::Reserve @ 0x82ADA3B8
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<sslOBJECT *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
