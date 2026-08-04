#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h"
#include "../../headers/ws/ai/aiBRAIN_frontier.h"
#include "../../headers/ws/nav/navSYS.h"

// aiPLANNER::SetNavSystemToUnit(aiBRAIN*, const dsTSTRING<char>&) — 0x83167B48
// Assign a named nav-system to a unit. The literal name "AUTO" means "auto-localize" (defer to
// LocalizeToNavSystem); any other name is looked up and, if found, pushed onto the brain's
// nav-goal object; a miss is logged. (The PPC decompiler typed the aiBRAIN* param as m3dV* and
// walked its internals by raw offset — reconstructed via the documented aiBRAIN_frontier helpers.)
void aiPLANNER::SetNavSystemToUnit(aiBRAIN *pBrain, const dsTSTRING<char> &nsName)
{
    const char *name = nsName.pBuffer->str;

    // Inlined strcmp(name, "AUTO").
    const char *want = "AUTO";
    int diff;
    do
    {
        int c = (unsigned char)*name;
        diff = c - (unsigned char)*want;
        if (c == 0)
            break;
        ++name;
        ++want;
    } while (diff == 0);

    if (diff == 0)
    {
        // Name is exactly "AUTO".
        LocalizeToNavSystem(pBrain);
        return;
    }

    navSYS *ns = navManager->FindNavSys(nsName.pBuffer->str);
    if (ns)
    {
        aiNavGoal_SetNavSystem(aiBRAIN_NavGoal(pBrain), ns, true);
    }
    else
    {
        _apLog("~AI,Error~%s: Attention! Can't find nav-system '%s'",
               aiBRAIN_DbgName(pBrain), nsName.pBuffer->str);
    }
}
