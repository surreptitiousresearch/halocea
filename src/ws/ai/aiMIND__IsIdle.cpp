#include "../../headers/ws/ai/aiMIND.h"

// aiMIND::IsIdle @ 0x83227FB8
// ?IsIdle@aiMIND@@QBA_NXZ
//
// The mind is idle when its interop/social (isa) module is neither the "zero" arm nor alerted.
bool aiMIND::IsIdle() const
{
    return !isa.pointee->IsZero() && !isa.pointee->IsAlerted();
}
