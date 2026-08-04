#include "dsVECTOR.h"

// hcex bridge breakable-surface record; 100 bytes per the DB (bsp@0, idx@4, pInst@8,
// cd@12 (scnINST_CREATE_DATA, 88 bytes)).
struct hcexBREAKABLE_SURF; // boundary

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::_internalRealloc @ 0x823B0C10
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the 100-byte element as trivially relocatable here, matching Realloc).
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::_internalRealloc(int size)
{
    this->pData = (hcexBREAKABLE_SURF *)dlRealloc(this->pData, 100 * size, this->__cl.file, this->__cl.line);
}
