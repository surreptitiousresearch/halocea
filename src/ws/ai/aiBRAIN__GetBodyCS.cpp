#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // MDL_ST_PROP_WLN, aiGetBodyCS
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"

// aiBRAIN::GetBodyCS @ 0x8317A870
// ?GetBodyCS@aiBRAIN@@IAAXAAUm3dV@@00@Z
//
// Fetch the body's world-space coordinate system (position / forward / up). The body's cached
// property bit-array is queried for the "weapon-line" state, which selects how aiGetBodyCS derives
// the frame from the game-info and the body FSM.
void aiBRAIN::GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp)
{
    aiBODY_IFACE *pBody = body.pointee;
    // GetRefPropCache now returns const ctrlPROP_LIST*; its bytes are exactly the 12-cell word array.
    const ctrlPROP_LIST *propCache = pBody->GetRefPropCache();
    bool weaponLine = dsBIT_ARRAY_IMPL::Is(
        MDL_ST_PROP_WLN.id, reinterpret_cast<const unsigned int *>(propCache), 12);
    propFSM *fsm = pBody->GetBodyProp();
    propGAME_INFO *gameInfo = propGameInfo();
    aiGetBodyCS(gameInfo, fsm, weaponLine, pos, dir, dirUp);
}
