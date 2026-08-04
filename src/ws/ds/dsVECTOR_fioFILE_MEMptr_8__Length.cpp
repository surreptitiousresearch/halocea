#include "dsVECTOR.h"

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::Length @ 0x82AC7378
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<fioFILE_MEM *, 8>::Length() const
{
    return this->nElem;
}
