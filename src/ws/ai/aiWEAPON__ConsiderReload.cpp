#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::ConsiderReload() @ 0x832B3740
// ?ConsiderReload@aiWEAPON@@IAAXPAVpropWPN_AI_USABLE@@H@Z
//
// Instantly reload `ctrl`'s weapon when it exists and its clip has dropped to/below `least_percent`.
void aiWEAPON::ConsiderReload(propWPN_AI_USABLE *ctrl, int least_percent)
{
    if (ctrl)
    {
        if (!rld.IsLoaded(ctrl, least_percent))
            ReloadInstantly(ctrl);
    }
}
