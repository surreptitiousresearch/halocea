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

// aiPLANNER::implAddEnemy(iaIACTOR*) @ 0x832488B0
// ?implAddEnemy@aiPLANNER@@UAA_NPAViaIACTOR@@@Z  (virtual)
// SSL op: register `actor` as an active enemy entity for the AI. Brains and players are already
// enemies by their team relationships, so passing one is rejected with a diagnostic; a null actor
// is likewise rejected. The return value is unconditionally 0.
bool aiPLANNER::implAddEnemy(iaIACTOR *actor)
{
    if (!actor)
    {
        _apLog("~AI,Error~AddEnemy(): argument is NULL");
    }
    else if (aiIsBrain(actor, nullptr))
    {
        _apLog("~AI,Error~AddEnemy(): argument is aiBRAIN. Brains are already enemies, they "
               "shouldn't be added directly.");
    }
    else if (gssIsPlayer(actor))
    {
        _apLog("~AI,Error~AddEnemy(): argument is player. Players are already enemies, they "
               "shouldn't be added directly.");
    }
    else
    {
        wah.RegisterActiveEnt(actor);
    }
    return false;
}
