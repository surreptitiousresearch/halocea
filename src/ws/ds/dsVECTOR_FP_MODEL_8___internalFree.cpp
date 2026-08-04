#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<FP_MODEL,8>::_internalFree @ 0x823CFC68
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<FP_MODEL, 8>::_internalFree(void *p)
{
    dlFree(p);
}
