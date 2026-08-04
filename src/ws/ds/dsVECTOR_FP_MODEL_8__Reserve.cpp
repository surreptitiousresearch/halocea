#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

// dsVECTOR<FP_MODEL,8>::Reserve @ 0x823E0240
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<FP_MODEL, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
