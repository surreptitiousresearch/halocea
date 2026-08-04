#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

// dsVECTOR<FP_MODEL,8>::Clear @ 0x823CF850
// Destroy every live element and reset the count. FP_MODEL is trivially destructible (a plain POD
// record — pInst is an unowned observer pointer), so the per-element destructor loop is empty
// and collapses to nElem=0.
template<>
void dsVECTOR<FP_MODEL, 8>::Clear()
{
    this->nElem = 0;
}
