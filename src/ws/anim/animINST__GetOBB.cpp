#include "animINST.h"

// animINST::GetOBB @ 0x825E4E30
// Return the instance's oriented bounding box, revalidating it first unless the caller forces
// the cached value or the OBB validity bit (stateVld & 0x100) is not set.
m3dOBB *animINST::GetOBB(int forceNoValidate)
{
    if (!forceNoValidate && (this->stateVld & 0x100) != 0)
        this->Validate(256);
    return &this->obb;
}
