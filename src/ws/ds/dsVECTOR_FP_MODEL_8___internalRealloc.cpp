#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<FP_MODEL,8>::_internalRealloc @ 0x823D0270
// Resize the backing store to `size` elements via the debug reallocator. FP_MODEL is trivially
// relocatable, so this is a bare dlRealloc. Element size 24 bytes.
template<>
void dsVECTOR<FP_MODEL, 8>::_internalRealloc(int size)
{
    this->pData = (FP_MODEL *)dlRealloc(this->pData, 24 * size, this->__cl.file, this->__cl.line);
}
