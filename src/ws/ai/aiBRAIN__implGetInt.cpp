#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// The ai08 planner singleton (aiEXIFACE.h). Declared locally to avoid its header's conflicting
// gsTEAMS boundary definition.
extern aiPLANNER *aiPlanner;
// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// The interned int SSL parameter names (a8 AI namespace). boundary.
namespace AI {
    extern const dsSTRID GETI_SQUAD_MEMBERS; // ?GETI_SQUAD_MEMBERS@AI@@2VdsSTRID@@B
}

// aiBRAIN::implGetInt(const dsTSTRING<char>&) @ 0x83248EE0
// ?implGetInt@aiBRAIN@@UAAHABV?$dsTSTRING@D@@@Z  (virtual)
// SSL op: read the int SSL parameter named `desc`. The only supported name is "squad_members",
// which returns the size of the brain's coordinated group; any other name logs and returns 0.
int aiBRAIN::implGetInt(const dsTSTRING<char> &desc)
{
    dsSTRID id(desc, true); // existOnly
    if (AI::GETI_SQUAD_MEMBERS.id == id.id)
        return (&aiPlanner->coordinator)->GetGroupMembersCount(this);

    _apLog("~AI,Ssl,Error~'%s' GetInt(\"%s\"): unknown parameter name",
           names.nameInst, desc.pBuffer->str);
    return 0;
}
