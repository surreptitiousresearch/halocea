#include "animINST.h"

// animINST::GetBBox @ 0x825E4BC8
// Return the instance's axis-aligned bounding box, revalidating it first unless the caller
// forces the cached value.
m3dBOX *animINST::GetBBox(int forceNoValidate)
{
    if (!forceNoValidate)
        this->Validate(256);
    return &this->bbox;
}
