#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<camCAMERA*,8>::_internalRealloc @ 0x82703BD0
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<camCAMERA *, 8>::_internalRealloc(int size)
{
    this->pData = (camCAMERA **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
