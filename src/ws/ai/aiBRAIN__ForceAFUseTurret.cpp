#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/bhp/bhpTURRET_PAR.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/gm/gmPLACE_CHECKER.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"

// aiBRAIN::ForceAFUseTurret(dsSHARED_PTR<smtSMART_OBJ_BASE,...>, const dsTSTRING<char>&, bool, bool)
//   @ 0x83286000
// ?ForceAFUseTurret@aiBRAIN@@...
//
// "Force auto-fly" onto a turret: pick the nearest available+usable entry point of `smtTurret`,
// teleport (Transport) the brain's body there, then build a TURRET behaviour named "<bhvName>_TUR"
// and set/push it on the best-behaviour module. On success, notify the task system + OnBhvEnd hooks
// and return true; on any failure (all entries locked / unavailable / behaviour rejected) log and
// return false.

void _apLog(const char *fmt, ...);

bool aiBRAIN::ForceAFUseTurret(
        dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtTurret,
        const dsTSTRING<char> &bhvName, bool checkAngles, bool doPushBHV)
{
    // Collect the turret's currently-available entry-point slots.
    dsVECTOR<smtSMT_ENTER_INFO, 8> slots;      // b8_turret.cpp:119 debug alloc tag
    smtTurret.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

    if (!slots.nElem)
    {
        _apLog("~AI,Error~'%s' ForceAF call UseTurret(): all entry points of turret '%s' are locked",
               names.nameInst, smtTurret.pointee->name.CStr());
        return false;
    }

    // Pick the reachable+usable entry point closest to the brain.
    int   bestIdx = -1;
    float bestDist = 2000000.0f;
    for (int i = 0; i < slots.nElem; ++i)
    {
        smtSMT_ENTER_INFO *slot = &slots[i];
        // DEVIATION: decompiler dispatches CanUseSmart through body.pointee's vtbl with the shared
        // ptr + entry info; body is a boundary interface. Modeled at semantic level.
        if (body.pointee->CanUseSmart(&smtTurret, &slot->info))
        {
            m3dV pos;
            slot->placeEnter->trk.GetPos(&pos);
            float d = this->Dist_2(&pos);
            if (d < bestDist)
            {
                bestDist = d;
                bestIdx  = i;
            }
        }
    }

    if (bestIdx < 0)
    {
        _apLog("~AI,Error~'%s' ForceAF call UseTurret(): all entry points of turret '%s' are unavailable",
               names.nameInst, smtTurret.pointee->name.CStr());
        return false;
    }

    // Teleport the body to the chosen entry point.
    smtSMT_ENTER_INFO *best = &slots[bestIdx];
    m3dV enterPos;
    best->placeEnter->trk.GetPos(&enterPos);
    Transport(&enterPos);

    // Build the TURRET behaviour parameters.
    // DEVIATION: decompiler default-constructs a bhpSMART_PAR then patches the vtable to
    // bhpTURRET_PAR; it is a bhpTURRET_PAR. The isCheckForAiming field (v62 @ +0x3C) = checkAngles.
    bhpTURRET_PAR turretPar;
    turretPar.typeBehav       = AI_BID_TURRET;
    turretPar.smtObj          = smtTurret;      // shared-ptr copy (manual pointee/counter bump inlined)
    turretPar.entryPointInfo  = best->info;
    turretPar.isCheckForAiming = checkAngles;

    // Behaviour name "<bhvName>_TUR".
    dsTSTRING<char> bhvTurretName = bhvName;
    bhvTurretName.Insert(bhvTurretName.pBuffer->strLen, "_TUR", -1);

    // Set or push the behaviour on the best-behaviour module.
    // DEVIATION: decompiler calls Push/SetBehaviorByParams through the aiBEST_BASE vtbl with args
    // stripped; best is a boundary module. Modeled with the turret params + behaviour name.
    if (doPushBHV)
    {
        if (!mind.best.pointee->PushBehaviorByParams(&turretPar, 0, bhvTurretName.pBuffer->str))
        {
            _apLog("~AI,Ssl~'%s' UseTurret(\"%s\", \"%s\"): can't push TURRET bhv",
                   names.nameInst, smtTurret.pointee->name.CStr(), bhvName.pBuffer->str);
            return false;
        }
    }
    else if (!mind.best.pointee->SetBehaviorByParams((bhpPARAMS *)&turretPar, bhvTurretName.pBuffer->str))
    {
        _apLog("~AI,Ssl~'%s' UseTurret(\"%s\", \"%s\"): can't set TURRET bhv",
               names.nameInst, smtTurret.pointee->name.CStr(), bhvName.pBuffer->str);
        return false;
    }

    _apLog("~AI~'%s' ForceAF call UseTurret(\"%s\", \"%s\")",
           names.nameInst, smtTurret.pointee->name.CStr(), bhvName.pBuffer->str);

    if (bhvName.pBuffer->strLen)
    {
        mind.task.pointee->NoticeBhvEndSuccess(0x1000000u, bhvName);
        OnBhvEnd(bhvName.pBuffer->str);
        OnBhvEndSuccess(bhvName.pBuffer->str);
    }

    return true;
    // turretPar / slots / bhvTurretName / smtTurret destructors release at scope end.
}
