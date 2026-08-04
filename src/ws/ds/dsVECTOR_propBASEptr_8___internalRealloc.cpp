#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<propBASE*,8>::_internalRealloc @ 0x826C2110
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<propBASE *, 8>::_internalRealloc(int size)
{
    this->pData = (propBASE **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
