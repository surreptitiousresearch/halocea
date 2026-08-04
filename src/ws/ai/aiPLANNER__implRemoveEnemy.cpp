#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ia/iaIACTOR.h"

struct aiBRAIN_IFACE;
struct msgADDR;

// ?aiIsBrain@@YA_NPAViaIACTOR@@PAPAVaiBRAIN_IFACE@@@Z @ 0x82D31C98 — true when `actor` is an AI
// brain (optionally returning it through `outBrain`). boundary.
extern bool aiIsBrain(iaIACTOR *actor, aiBRAIN_IFACE **outBrain);
// ?gssIsPlayer@@YAHPBVmsgADDR@@@Z @ 0x829DFB20 — non-zero when `actor` is a player. boundary.
extern int gssIsPlayer(const msgADDR *actor);
// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiPLANNER::implRemoveEnemy(iaIACTOR*) @ 0x83248958
// ?implRemoveEnemy@aiPLANNER@@UAAXPAViaIACTOR@@@Z  (virtual)
// SSL op: unregister `actor` as an active enemy entity. Brains and players cannot be removed this
// way (they are enemies by team relationship); a null actor is rejected too.
void aiPLANNER::implRemoveEnemy(iaIACTOR *actor)
{
    if (!actor)
    {
        _apLog("~AI,Error~RemoveEnemy(): argument is NULL");
    }
    else if (aiIsBrain(actor, nullptr))
    {
        _apLog("~AI,Error~RemoveEnemy(): argument is aiBRAIN. They shouldn't be removed directly.");
    }
    else if (gssIsPlayer(actor))
    {
        _apLog("~AI,Error~RemoveEnemy(): argument is player. They shouldn't be removed directly.");
    }
    else
    {
        wah.UnRegisterActiveEnt(actor);
    }
}
