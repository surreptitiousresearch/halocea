#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::Reload() @ 0x832B3268
// ?Reload@aiWEAPON@@IAAXPAVpropWPN_AI_USABLE@@@Z
//
// Full reload of controller `ctrl`: consume a clip on the controller (the ammo side), reload the
// weapon instantly, then notify the owning brain that a reload happened (SSL OnReload event).
void aiWEAPON::Reload(propWPN_AI_USABLE *ctrl)
{
    if (ctrl)
    {
        ctrl->Reload();
        ReloadInstantly(ctrl);
        pBrain->OnReload();
    }
}
