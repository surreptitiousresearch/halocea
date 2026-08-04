#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<cdtBONE*,8>::_internalRealloc @ 0x828B8CE8
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<cdtBONE *, 8>::_internalRealloc(int size)
{
    this->pData = (cdtBONE **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
