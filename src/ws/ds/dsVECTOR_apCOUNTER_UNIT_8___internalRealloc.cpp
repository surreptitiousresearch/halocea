#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<apCOUNTER_UNIT,8>::_internalRealloc @ 0x82714228
// apCOUNTER_UNIT is 48 bytes; its owned dsTSTRING<char> name is a ref-counted handle whose
// refcount lives in the pointed-to buffer, so moving the struct's bytes is safe — bare dlRealloc.
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::_internalRealloc(int size)
{
    this->pData = (apCOUNTER_UNIT *)dlRealloc(this->pData, sizeof(apCOUNTER_UNIT) * size, this->__cl.file, this->__cl.line);
}
