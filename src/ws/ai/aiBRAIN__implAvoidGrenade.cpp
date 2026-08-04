// aiBRAIN::implAvoidGrenade @ 0x83279598
// ?implAvoidGrenade@aiBRAIN@@UAA_NHABV?$dsTSTRING@D@@M@Z  (virtual)
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/bhp/bhpAVOID_PAR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);
// Level-time accumulator (gs) — used as the log timestamp. boundary.
extern float gsElapsedTimeLevel;

// SSL op: push a grenade-avoidance behaviour for the specific in-flight grenade `grenadeId`, using
// the reaction-list named `reactions` and a `timerDelayToStart` pre-reaction delay. No-op (returns
// false) when the SSL common back-off gate rejects the call or the grenade is no longer tracked.
bool aiBRAIN::implAvoidGrenade(int grenadeId, const dsTSTRING<char> &reactions, float timerDelayToStart)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> backoffFlags{1};
    // The original passes `reactions` as an (unused) vararg to a format with no conversions.
    if (IsSSLCommonBackOff(*dsSPrintf(&callName, "AvoidGrenade(\"\")"), backoffFlags))
        return false;

    if (!aiPlanner)
        return false;

    // aiPLANNER::grass is modeled as opaque bytes (0x1128); it is an aiGRASS in the binary.
    aiGRASS *grass = (&aiPlanner->grass);
    const aiGRASS::GRENADE_INFO *grInfo = grass->GetGrenadeInfo(grenadeId);
    if (!grInfo)
        return false;

    st.pointee->perc.grenadeMPIDs.PushBack(grenadeId);

    bhpAVOID_PAR params;
    params.Init(grInfo, reactions, timerDelayToStart);
    _apLog("~AI,Anim,Weapon~[%10.2f]'%s': grenade avoidance: pushed", gsElapsedTimeLevel, names.nameInst);
    mind.best.pointee->PushBehaviorByParams(&params, 1, "GRENADE_AVOIDANCE");
    return true;
}
