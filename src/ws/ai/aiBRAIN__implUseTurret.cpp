#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/bhp/bhpTURRET_PAR.h"
#include "../../headers/ws/bhp/bhpPATH_PAR.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::implUseTurret(const dsTSTRING<char>&, const dsTSTRING<char>&, bool, bool) @ 0x83286590
// ?implUseTurret@aiBRAIN@@...
//
// Mount/operate the turret smart-object named `smtTurretName` with behaviour `bhvName`. Runs the
// "UseTurret" SSL common back-off gate, resolves the smart object, validates that its entity carries
// a WPN_AI_USABLE (weapon) property, then either force-flies onto it (ForceAFUseTurret, when the
// brain is in the ForceAF mind state — mind flag bit 25) or builds a nav path to the turret
// (bhpPATH_PAR via the goal module) and sets/pushes a "<bhvName>_TUR" turret behaviour on top.

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ
extern const dsSTRID AI_PATH_SPEED_TYPE_AUTO;   // AI::PATH_SPEED_TYPE_AUTO  boundary
extern const dsTYPE_ID propWPN_AI_USABLE_DESC_TYPE_ID; // propWPN_AI_USABLE_DESC::TYPE_ID  boundary
extern const dsTYPE_ID propCONTAINER_DESC_TYPE_ID;     // propCONTAINER_DESC::TYPE_ID      boundary

// Locate a WPN_AI_USABLE property on the entity's property/component lists.
static bool entityHasWpnAiUsable(propCONTAINER *pc)
{
    if (pc->typeId.IsDerivedFrom(propWPN_AI_USABLE_DESC_TYPE_ID))
        return true;
    if (!pc->typeId.IsDerivedFrom(propCONTAINER_DESC_TYPE_ID))
        return false;
    for (int i = 0; i < pc->properties.nElem; ++i)
        if (pc->properties[i]->GetProperty<propWPN_AI_USABLE>())
            return true;
    for (int i = 0; i < pc->components.nElem; ++i)
        if (pc->components[i]->GetProperty<propWPN_AI_USABLE>())
            return true;
    return false;
}

bool aiBRAIN::implUseTurret(const dsTSTRING<char> &smtTurretName, const dsTSTRING<char> &bhvName,
                            bool checkAngles, bool doPushBHV)
{
    // SSL common back-off gate.
    {
        dsTSTRING<char> callName;
        dsFLAGS<SSL_COMMON_BACKOFF, int> backoffFlags{3};
        if (IsSSLCommonBackOff(*dsSPrintf(&callName, "UseTurret(\"%s\", \"%s\")",
                                          smtTurretName.pBuffer->str, bhvName.pBuffer->str),
                               backoffFlags))
            return false;
    }

    // Empty turret name -> nothing to do.
    if (smtTurretName.pBuffer->str[0] == '\0')
        return false;

    // Resolve the smart object.
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtTurret =
        aiPlanner->FindSmartObject(smtTurretName);
    if (!smtTurret.pointee)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call UseTurret(\"%s\", \"%s\"): can't find smart object",
               names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    // The smart object's entity must carry an AI-usable weapon property.
    entENTITY *ent = smtTurret.pointee->entHolder.pHandle
                         ? smtTurret.pointee->entHolder.pHandle->pPtr
                         : nullptr;
    if (!entityHasWpnAiUsable(&ent->propContainer))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call UseTurret(\"%s\", \"%s\"): smart object is not weapon "
               "(property WPN_AI_CTRL is absent)",
               names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    // ForceAF mind state (mind flag bit 25): fly straight onto the turret.
    if ((st.pointee->mind.val >> 25) & 1)
    {
        dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtCopy = smtTurret;
        return ForceAFUseTurret(smtCopy, bhvName, checkAngles, doPushBHV);
    }

    // Normal path: build a nav path to the turret, then a turret behaviour on top of it.
    dsPAIR<dsSTRID, dsSTRID> entryPointInfo;
    bhpPATH_PAR pathPar;
    // DEVIATION: BuildPathToSmtObj dispatched through the goal module (aiGOAL_BASE boundary) with the
    // shared ptr, the path blob, and the out entry-point pair.
    // pathPar.pathData is modeled as an opaque byte blob; it is a navPATH in the binary.
    if (!mind.goal.pointee->BuildPathToSmtObj(smtTurret, (navPATH *)pathPar.pathData, entryPointInfo, 0))
    {
        _apLog("~AI,Ssl,Error~'%s' UseTurret(\"%s\", \"%s\"): can't build path",
               names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    pathPar.isLoop  = 0;
    pathPar.isShoot = 1;
    {
        dsTSTRING<char> speedName;
        speedName.UnsafeInit(AI_PATH_SPEED_TYPE_AUTO.CStr(), -1, 0);
        pathPar.ParseSpeed(this, &speedName);
    }

    bhpTURRET_PAR turretPar;
    turretPar.smtObj          = smtTurret;
    turretPar.isCheckForAiming = checkAngles;
    turretPar.entryPointInfo  = entryPointInfo;

    // Behaviour name "<bhvName>_TUR"; the path behaviour keeps the original name.
    dsTSTRING<char> bhvTurretName = bhvName + "_TUR";
    const char *pathBhvName = bhvName.pBuffer->strLen ? bhvName.pBuffer->str : nullptr;

    // Notify the task system for both behaviours when the brain runs a scripted (0x1000000) task.
    if (st.pointee->mind.val & 0x1000000)
    {
        turretPar.notifyTaskSys = 1;
        pathPar.notifyTaskSys   = 1;
    }

    // Set or push the turret behaviour.
    // DEVIATION: Push/SetBehaviorByParams dispatched through the aiBEST_BASE vtbl with args stripped.
    if (doPushBHV)
    {
        if (!mind.best.pointee->PushBehaviorByParams(&turretPar, 0, bhvTurretName.pBuffer->str))
        {
            _apLog("~AI,Ssl~'%s' UseTurret(\"%s\", \"%s\"): can't push TURRET bhv",
                   names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
            return false;
        }
    }
    else if (!mind.best.pointee->SetBehaviorByParams((bhpPARAMS *)&turretPar, bhvTurretName.pBuffer->str))
    {
        _apLog("~AI,Ssl~'%s' UseTurret(\"%s\", \"%s\"): can't set TURRET bhv",
               names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    // Push the movement path behaviour underneath.
    if (!mind.best.pointee->PushBehaviorByParams(&pathPar, 0, pathBhvName))
    {
        _apLog("~AI,Ssl~'%s' UseTurret(\"%s\", \"%s\"): can't push PATH bhv",
               names.nameInst, smtTurretName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    return true;
    // pathPar / turretPar / bhvTurretName / smtTurret / entryPointInfo destructors release at scope end.
}
