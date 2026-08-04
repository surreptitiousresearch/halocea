#include "../../headers/ws/ai/aiBRAIN.h"

// aiBRAIN::ProcessINIT @ 0x83179CA8
// ?ProcessINIT@aiBRAIN@@UAAHPAX@Z
//
// Per-spawn brain init. Run the base entity init and require an animated instance plus a
// successful any-health init; then take the live- or dead-spawn mind path. Returns 1 on success.
int aiBRAIN::ProcessINIT(void *pInfo)
{
    if (!entENTITY::ProcessINIT(pInfo) || !pInst || !ProcessINITAnyHealth())
        return 0;

    if (mind.IsLiveOnInit())
    {
        mind.ProcessINITLive();
        CopyDataFromDescToStatus();
    }
    else
    {
        ProcessINITDead();
    }
    return 1;
}
