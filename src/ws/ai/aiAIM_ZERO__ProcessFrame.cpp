#include "../../headers/ws/ai/aiAIM_ZERO.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // operator*(float,m3dV), _m3dCheckValid

// aiAIM_ZERO::ProcessFrame @ 0x83299E10
// ?ProcessFrame@aiAIM_ZERO@@UAAXXZ
//
// The null aim module aims 300 units straight ahead of the body (computed once, latched via
// isFilled), then publishes that point through the shared FillResults path.
void aiAIM_ZERO::ProcessFrame()
{
    aiSTATUS *status = pBrain->st.pointee;
    if (!isFilled)
    {
        m3dV forward = 300.0f * status->cs.dirBody;
        _m3dCheckValid(&status->cs.posBody);
        _m3dCheckValid(&forward);
        isFilled = true;
        posStraight.x = status->cs.posBody.x + forward.x;
        posStraight.y = status->cs.posBody.y + forward.y;
        posStraight.z = status->cs.posBody.z + forward.z;
    }
    FillResults(posStraight, posStraight);
}
