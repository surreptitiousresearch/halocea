#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

// dsVECTOR<FP_MODEL,8>::Length @ 0x823CF3F8
// Number of live elements.
template<>
int dsVECTOR<FP_MODEL, 8>::Length() const
{
    return this->nElem;
}
