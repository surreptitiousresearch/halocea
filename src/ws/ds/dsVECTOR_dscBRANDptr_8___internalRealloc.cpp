#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dscBRAND*,8>::_internalRealloc @ 0x82663300
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<dscBRAND *, 8>::_internalRealloc(int size)
{
    this->pData = (dscBRAND **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
