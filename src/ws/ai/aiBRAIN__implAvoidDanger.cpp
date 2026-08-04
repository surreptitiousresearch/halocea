// aiBRAIN::implAvoidDanger @ 0x83278B20
// ?implAvoidDanger@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z  (virtual)
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiDANGER.h"
#include "../../headers/ws/bhp/bhpAVOID_PAR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// SSL op: react to the AI's current danger by pushing a grenade-avoidance behaviour, using the
// reaction-list named `reactions`. No-op (returns false) when the SSL common back-off gate rejects
// the call, when there is no active danger, when the danger is not a grenade, or when the grenade is
// no longer tracked by the planner's grass.
bool aiBRAIN::implAvoidDanger(const dsTSTRING<char> &reactions)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> backoffFlags{1};
    if (IsSSLCommonBackOff(*dsSPrintf(&callName, "AvoidDanger(\"%s\")", reactions.CStr()), backoffFlags))
        return false;

    aiDANGER *danger = st.pointee->danger.pointee;
    if (!danger || danger->type)          // only grenade danger (AI_DNG_GRENADE == 0) is handled here
        return false;
    if (!aiPlanner)
        return false;

    // aiPLANNER::grass is modeled as opaque bytes (0x1128); it is an aiGRASS in the binary.
    aiGRASS *grass = (&aiPlanner->grass);
    const aiGRASS::GRENADE_INFO *grInfo = grass->GetGrenadeInfo(danger->grenadeMPID);
    if (!grInfo)
        return false;

    bhpAVOID_PAR params;
    params.grenade.SetPtr(const_cast<entENTITY *>(grInfo->grenade));
    params.posStart  = grInfo->posStart;
    params.posCDT1st = grInfo->posEnd;
    mind.best.pointee->PushBehaviorByParams(&params, 1, "GRENADE_AVOIDANCE");
    return true;
}
