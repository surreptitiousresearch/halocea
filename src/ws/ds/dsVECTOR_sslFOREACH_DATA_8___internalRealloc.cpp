#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslFOREACH_DATA,8>::_internalRealloc @ 0x82AF0520
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the element as trivially relocatable here, matching Realloc). Element size 8.
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::_internalRealloc(int size)
{
    this->pData = (sslFOREACH_DATA *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
