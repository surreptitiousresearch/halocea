// haloENGINE_CONTROL__InitSettings @0x823CC698
#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::InitSettings @ 0x823CC6B0 — apply both primary and secondary users'
// persisted control settings, then bind the event dispatcher to this object.
void haloENGINE_CONTROL::InitSettings()
{
    ApplySettings(true);
    ApplySettings(false);
    eventDispatcher.SetProduct(this);
}
