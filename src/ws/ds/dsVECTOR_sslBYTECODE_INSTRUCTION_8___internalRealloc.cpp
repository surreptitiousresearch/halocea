#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::_internalRealloc @ 0x83087DF0
// Resize the backing store to `size` elements via the debug reallocator. sslBYTECODE_INSTRUCTION
// is a trivially relocatable POD, so this is a bare dlRealloc. Element size 8 bytes.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::_internalRealloc(int size)
{
    this->pData = (sslBYTECODE_INSTRUCTION *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
