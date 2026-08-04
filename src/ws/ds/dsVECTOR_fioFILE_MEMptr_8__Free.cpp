#include "dsVECTOR.h"

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::Free @ 0x82AC8A10
// Release the backing storage entirely (Realloc to zero capacity).
template<>
void dsVECTOR<fioFILE_MEM *, 8>::Free()
{
    this->Realloc(0);
}
