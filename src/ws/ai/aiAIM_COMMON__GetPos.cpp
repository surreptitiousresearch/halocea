#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero

// aiAIM_COMMON::GetPos @ 0x83299A88
// ?GetPos@aiAIM_COMMON@@UBA?AUm3dV@@XZ  (const, returns m3dV by value)
//
// The current aim position: the tracked target's center when an aim is assigned, else the zero
// vector.
m3dV aiAIM_COMMON::GetPos() const
{
    m3dV result = m3dVZero;
    if (IsGiven())
        trkAim.GetCenter(&result);
    return result;
}
