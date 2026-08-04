#include "animINST.h"
#include "animMNG.h"

// animINST::ApplyAnim @ 0x82B2BD18 (private)
// If the instance has a model object and an animation manager, dispatch the manager's virtual
// ApplyAnim (vtbl slot +0x38). The caller passes 1 (r4=1); the decompiler dropped that arg.
void animINST::ApplyAnim()
{
    if (this->pObj) {
        animMNG *pAnimMng = this->pAnimMng;
        if (pAnimMng)
            pAnimMng->ApplyAnim(1);
    }
}
