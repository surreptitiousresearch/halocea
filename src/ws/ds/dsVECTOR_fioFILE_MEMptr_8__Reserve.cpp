#include "dsVECTOR.h"

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::Reserve @ 0x82AC89F8
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<fioFILE_MEM *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
