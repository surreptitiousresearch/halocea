#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<rendSR_LAMP_DATA,8>::_internalRealloc @ 0x8252C070
// Resize the backing store to `size` elements via the debug reallocator. rendSR_LAMP_DATA is a
// trivially relocatable packed word, so this is a bare dlRealloc. Element size 4 bytes.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::_internalRealloc(int size)
{
    this->pData = (rendSR_LAMP_DATA *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
