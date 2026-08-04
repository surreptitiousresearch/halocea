#include "../../headers/ws/ai/aiCON_CB.h"

// aiCON_CB::SendChanges @ 0x83182678
// Per-frame delta flush: push any changed SSL-var/main/extra/task/behaviour sub-lists, and once
// the ~1s throttle timer elapses also push the AI unit count. Returns false on the first
// sub-send failure.
bool aiCON_CB::SendChanges(float dt)
{
    if (!this->SendListSSLVars(false)) return false;
    if (!this->SendMainList(false))    return false;
    if (!this->SendExtraList(false))   return false;
    if (!this->SendTaskSys(false))     return false;
    bool ok = this->SendBhvStack(false);

    float remaining = this->timer - dt;
    this->timer = remaining;
    if (remaining <= 0.000001f)
    {
        this->timer = 1.0f;
        if (!ok)
            return false;
        if (!this->SendAICountToConsole(false))
            return false;
        return true;
    }
    return ok;
}
