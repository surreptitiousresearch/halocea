#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"       // aiGOAL_BASE::IsAcceptHO
#include "../../headers/ws/ai/aiPLANNER.h"         // aiPLANNER::ReconsiderHOChecker
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // aiPLANNER *aiPlanner
#include "../../headers/ws/m3d/m3dV.h"

// aiHO_SYS::AdvCheckAcceptHO @ 0x832A5C78
// ?AdvCheckAcceptHO@aiHO_SYS@@UBA_NPBVnavWP@@P82@BA_NABUm3dV@@@ZAA_N@Z
//
// Ask this brain's goal module whether the HO at `howp` is acceptable. The caller's `checker` is
// accepted (per the ABI) but ignored: the planner's own reconsider-checker is substituted, with the
// enable mask fixed at 6. `checkAllEnemies` is passed through by reference.
bool aiHO_SYS::AdvCheckAcceptHO(const navWP *howp,
                               bool (aiHO_SYS::*checker)(const m3dV &) const,
                               bool &checkAllEnemies) const
{
    (void)checker; // ABI-present but unused: planner supplies the checker below (matches disasm)
    if (!howp)
        return false;

    // pBrain->mind.goal.pointee: aiBRAIN.mind (aiMIND) @0x1C8 + aiMIND.goal (dsSHARED_PTR) @0x28 +
    // pointee @0x0 = 0x1F0 (DB-verified; matches disasm lwz 0x1F0). Read via offset so this TU does
    // not depend on aiBRAIN.h's full (heavy) definition.
    aiGOAL_BASE *goal = *reinterpret_cast<aiGOAL_BASE **>(reinterpret_cast<char *>(pBrain) + 0x1F0);
    bool (aiHO_SYS::*reconsiderChecker)(const m3dV &) const = aiPlanner->ReconsiderHOChecker();
    return goal->IsAcceptHO(howp, reconsiderChecker, 6u, &checkAllEnemies);
}
