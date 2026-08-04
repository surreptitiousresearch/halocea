#include "../../headers/ws/ai/aiSND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ia/iaSND_PLAY_DATA.h"
#include "../../headers/ws/ds/LIST.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dLengthVector

// aiSND::ProcessPitch @ 0x832996B0
// ?ProcessPitch@aiSND@@IAAXXZ
//
// When the pitch response is enabled, look up the RPC-marked ("rpc") playing sound; if present, push
// the unit's current speed through the descriptor's sound-pitch curve and set the sound's "speed"
// RPC parameter to the resulting value.
void aiSND::ProcessPitch()
{
    if ((permissions.val & AI_SNDP_ENABLE_PITCH) == 0)
        return;

    static dsSTRID markRPC("rpc", false);

    ds::LIST<iaSND_PLAY_DATA>::ITERATOR start; // start.data = nullptr
    ds::LIST<iaSND_PLAY_DATA>::ITERATOR it = pBrain->FindMarkedSound(markRPC, start);
    ds::LIST<iaSND_PLAY_DATA>::DATA *node = it.data;
    if (!node)
        return;

    m3dV speed;
    pBrain->GetSpeed(&speed);
    float len = m3dLengthVector(&speed);

    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(pBrain->spDesc.pointee);
    float pitch;
    desc->curveSndPitch.GetValue(len, &pitch, nullptr);

    node->val.SetRPCParameter("speed", pitch);
}
