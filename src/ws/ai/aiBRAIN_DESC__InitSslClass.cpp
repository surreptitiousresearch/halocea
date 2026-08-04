#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiBRAIN.h"        // aiBRAIN::cb* SSL callbacks
#include "../../headers/ws/ai/aiPLANNER.h"       // aiPLANNER::RegisterBhvSslFunctions
#include "../../headers/ws/ai/aiCON_CB_globals.h" // global aiPlanner
#include "../../headers/ws/ent/entDESC.h"        // entDESC::InitSslClass (base)
#include "../../headers/ws/ssl/sslCLASS_REF.h"   // AddCbFunc / RegisterEvent / sslCB_GLOBAL_FN
#include "../../headers/ws/ssl/sslERROR.h"       // sslERROR
#include <string.h>                               // memcpy

// aiBRAIN_DESC::InitSslClass(const dsTSTRING<char>&) @ 0x8325BCB8
// ?InitSslClass@aiBRAIN_DESC@@UAAHABV?$dsTSTRING@D@@@Z
//
// One-time registration of the brain SSL class. Chains to the entDESC base init, and (guarded by
// isSslClassInited) unshares the shared sslClass, lets the global aiPlanner register its behaviour
// functions onto it, then registers 20 script events (RegisterEvent) followed by 118 native
// callback functions (AddCbFunc). Each id is stored into the sslEventId_/sslFuncId_ tail; the
// first registration error aborts (returning 0).
//
// The disassembly threads one running sslERROR temporary (v283) through every call: each
// `sslCLASS_REF::Register.../AddCbFunc` returns a by-value sslERROR that is assigned into it and its
// temporary immediately destroyed (the inlined refcount-decrement/dlFree pair) -- reproduced here
// as plain RAII (`err = ...`), which the compiler lowers to the same sequence. On any err.id != 0
// the function jumps to the shared cleanup (destroy v283) and returns 0; the fall-through returns 1.
//
// DEVIATIONS:
//  - sslClass is the sslDESC::sslClass sub-object at base offset 0x10 (inside the opaque entDESC
//    base span of aiBRAIN_DESC); taken by typed reference here rather than fabricating the base
//    layout.
//  - Each native callback is a non-virtual aiBRAIN member function; the disassembly registers it
//    through AddCbFunc with the host bound as the leading argument. Represented here via the
//    pointer-to-member (sslCB_MEMBER_FN) overload and a member-to-member reinterpret_cast, which is
//    well-formed C++ and equivalent to the binary's registration.
//  - The errHint passed to every call is the fixed empty-string byte (byte_8200155A), not `hintErr`.
// Reinterpret an aiBRAIN member-callback pointer as the sslCB_HOST member-callback type AddCbFunc
// expects. aiBRAIN's inheritance makes a direct pmf-to-pmf reinterpret_cast ill-formed under MSVC, so
// the pointer representation is copied bitwise -- matching the binary, which registers each callback
// by its plain code address with the host bound as the leading argument.
static sslCB_MEMBER_FN AsCbMemberFn(void (aiBRAIN::*fn)(int, dsDATA *, dsDATA &, sslOBJ_REF))
{
    sslCB_MEMBER_FN out = nullptr;
    memcpy(&out, &fn, sizeof(out));
    return out;
}

int aiBRAIN_DESC::InitSslClass(const dsTSTRING<char> &hintErr)
{
    static const char *const kEmptyErrHint = ""; // byte_8200155A

    // aiBRAIN_DESC's entDESC base is an opaque byte span in the header model, so the base init is
    // reached through a base-cast + qualified (non-virtual) call.
    if (!reinterpret_cast<entDESC *>(this)->entDESC::InitSslClass(hintErr))
        return 0;

    if (this->isSslClassInited.val)
        return 1;

    this->isSslClassInited.val = 1;
    this->UnshareSslClass(false, false);

    // sslDESC::sslClass @ base+0x10 (opaque entDESC base span).
    sslCLASS_REF &sslClass =
        *reinterpret_cast<sslCLASS_REF *>(reinterpret_cast<char *>(this) + 0x10);

    if (aiPlanner)
        aiPlanner->RegisterBhvSslFunctions(sslClass);

    sslERROR err;

    err = sslClass.RegisterEvent("OnBhvEnd(bhv : string)", 0, kEmptyErrHint, &this->sslEventId_OnBhvEnd);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnBhvEndFailed(bhv : string)", 0, kEmptyErrHint, &this->sslEventId_OnBhvEndFailed);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnBhvEndSuccess(bhv : string)", 0, kEmptyErrHint, &this->sslEventId_OnBhvEndSuccess);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnBhvEndTerminate(bhv : string)", 0, kEmptyErrHint, &this->sslEventId_OnBhvEndTerminate);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnAlert(name : string = \"\")", 0, kEmptyErrHint, &this->sslEventId_OnAlert);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnEnemySeen()", 0, kEmptyErrHint, &this->sslEventId_OnEnemySeen);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnReload()", 0, kEmptyErrHint, &this->sslEventId_OnReload);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnInit()", 0, kEmptyErrHint, &this->sslEventId_OnInit);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnWounded()", 0, kEmptyErrHint, &this->sslEventId_OnWounded);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnComplete()", 0, kEmptyErrHint, &this->sslEventId_OnComplete);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnKilledByPlayer(name : string = \"\")", 0, kEmptyErrHint, &this->sslEventId_OnKilledByPlayer);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnBattleChanged(isBattle : bool)", 0, kEmptyErrHint, &this->sslEventId_OnBattleChanged);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnEnter(name : string = \"\")", 0, kEmptyErrHint, &this->sslEventId_OnEnter);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnLeave(name : string = \"\")", 0, kEmptyErrHint, &this->sslEventId_OnLeave);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnReach(wp_name : string)", 0, kEmptyErrHint, &this->sslEventId_OnReach);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnEnterHO()", 0, kEmptyErrHint, &this->sslEventId_OnEnterHO);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnLeaveHO()", 0, kEmptyErrHint, &this->sslEventId_OnLeaveHO);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnStrikeStart(strike_name : string)", 0, kEmptyErrHint, &this->sslEventId_OnStrikeStart);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnStrikeEnd(strike_name : string)", 0, kEmptyErrHint, &this->sslEventId_OnStrikeEnd);
    if (err.id)
        return 0;

    err = sslClass.RegisterEvent("OnEnemyDamaged(amount : float, killed : int)", 0, kEmptyErrHint, &this->sslEventId_OnEnemyDamaged);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnCreate()",
                             AsCbMemberFn(&aiBRAIN::cbOnCreate),
                             kEmptyErrHint, &this->sslFuncId_OnCreate);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnDestroy()",
                             AsCbMemberFn(&aiBRAIN::cbOnDestroy),
                             kEmptyErrHint, &this->sslFuncId_OnDestroy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnFrameBegin()",
                             AsCbMemberFn(&aiBRAIN::cbOnFrameBegin),
                             kEmptyErrHint, &this->sslFuncId_OnFrameBegin);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnDbgOut()",
                             AsCbMemberFn(&aiBRAIN::cbOnDbgOut),
                             kEmptyErrHint, &this->sslFuncId_OnDbgOut);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("DropWeapon()",
                             AsCbMemberFn(&aiBRAIN::cbDropWeapon),
                             kEmptyErrHint, &this->sslFuncId_DropWeapon);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetEnemy(nameTrk : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetEnemy),
                             kEmptyErrHint, &this->sslFuncId_SetEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndEnemy()",
                             AsCbMemberFn(&aiBRAIN::cbEndEnemy),
                             kEmptyErrHint, &this->sslFuncId_EndEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndEnemySoft()",
                             AsCbMemberFn(&aiBRAIN::cbEndEnemySoft),
                             kEmptyErrHint, &this->sslFuncId_EndEnemySoft);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetEnemy() : object",
                             AsCbMemberFn(&aiBRAIN::cbGetEnemy),
                             kEmptyErrHint, &this->sslFuncId_GetEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("IsEnemyType(typeName : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbIsEnemyType),
                             kEmptyErrHint, &this->sslFuncId_IsEnemyType);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetAim(nameTrk : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetAim),
                             kEmptyErrHint, &this->sslFuncId_SetAim);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndAim()",
                             AsCbMemberFn(&aiBRAIN::cbEndAim),
                             kEmptyErrHint, &this->sslFuncId_EndAim);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetAim(): string",
                             AsCbMemberFn(&aiBRAIN::cbGetAim),
                             kEmptyErrHint, &this->sslFuncId_GetAim);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetSenses(id: string)",
                             AsCbMemberFn(&aiBRAIN::cbSetSenses),
                             kEmptyErrHint, &this->sslFuncId_SetSenses);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("Alert()",
                             AsCbMemberFn(&aiBRAIN::cbAlert),
                             kEmptyErrHint, &this->sslFuncId_Alert);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ResetAlerts()",
                             AsCbMemberFn(&aiBRAIN::cbResetAlerts),
                             kEmptyErrHint, &this->sslFuncId_ResetAlerts);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetSniperVision(enable: bool = true)",
                             AsCbMemberFn(&aiBRAIN::cbSetSniperVision),
                             kEmptyErrHint, &this->sslFuncId_SetSniperVision);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetApproach(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetApproach),
                             kEmptyErrHint, &this->sslFuncId_SetApproach);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndApproach()",
                             AsCbMemberFn(&aiBRAIN::cbEndApproach),
                             kEmptyErrHint, &this->sslFuncId_EndApproach);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ForceWar()",
                             AsCbMemberFn(&aiBRAIN::cbForceWar),
                             kEmptyErrHint, &this->sslFuncId_ForceWar);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ForcePeace()",
                             AsCbMemberFn(&aiBRAIN::cbForcePeace),
                             kEmptyErrHint, &this->sslFuncId_ForcePeace);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("AutoBattle()",
                             AsCbMemberFn(&aiBRAIN::cbAutoBattle),
                             kEmptyErrHint, &this->sslFuncId_AutoBattle);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ShootNow(type : string)",
                             AsCbMemberFn(&aiBRAIN::cbShootNow),
                             kEmptyErrHint, &this->sslFuncId_ShootNow);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EnableStrike(type : string)",
                             AsCbMemberFn(&aiBRAIN::cbEnableStrike),
                             kEmptyErrHint, &this->sslFuncId_EnableStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("DisableStrike(type : string)",
                             AsCbMemberFn(&aiBRAIN::cbDisableStrike),
                             kEmptyErrHint, &this->sslFuncId_DisableStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("LockStrike(type : string, time : float)",
                             AsCbMemberFn(&aiBRAIN::cbLockStrike),
                             kEmptyErrHint, &this->sslFuncId_LockStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CancelCurStrike()",
                             AsCbMemberFn(&aiBRAIN::cbCancelCurStrike),
                             kEmptyErrHint, &this->sslFuncId_CancelCurStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("DbgStrike(type : string)",
                             AsCbMemberFn(&aiBRAIN::cbDbgStrike),
                             kEmptyErrHint, &this->sslFuncId_DbgStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetWpn(type : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetWpn),
                             kEmptyErrHint, &this->sslFuncId_SetWpn);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetSpreadAngle(x : float)",
                             AsCbMemberFn(&aiBRAIN::cbSetSpreadAngle),
                             kEmptyErrHint, &this->sslFuncId_SetSpreadAngle);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetDamageCurve(curve : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetDamageCurve),
                             kEmptyErrHint, &this->sslFuncId_SetDamageCurve);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("AIGetCurWeaponClassName() : string",
                             AsCbMemberFn(&aiBRAIN::cbAIGetCurWeaponClassName),
                             kEmptyErrHint, &this->sslFuncId_AIGetCurWeaponClassName);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetContourShootingDist(dist12_3 : float, dist3_6 : float, dist6_9 : float, dist9_12 : float)",
                             AsCbMemberFn(&aiBRAIN::cbSetContourShootingDist),
                             kEmptyErrHint, &this->sslFuncId_SetContourShootingDist);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("HasAnyAvailableStrike() : bool",
                             AsCbMemberFn(&aiBRAIN::cbHasAnyAvailableStrike),
                             kEmptyErrHint, &this->sslFuncId_HasAnyAvailableStrike);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("IsLive() : bool",
                             AsCbMemberFn(&aiBRAIN::cbIsLive),
                             kEmptyErrHint, &this->sslFuncId_IsLive);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetPar(key: string, val : float)",
                             AsCbMemberFn(&aiBRAIN::cbSetPar),
                             kEmptyErrHint, &this->sslFuncId_SetPar);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetFloat(desc : string) : float",
                             AsCbMemberFn(&aiBRAIN::cbGetFloat),
                             kEmptyErrHint, &this->sslFuncId_GetFloat);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetFloat(desc : string, value : float)",
                             AsCbMemberFn(&aiBRAIN::cbSetFloat),
                             kEmptyErrHint, &this->sslFuncId_SetFloat);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetBool(desc : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbGetBool),
                             kEmptyErrHint, &this->sslFuncId_GetBool);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetInt(desc : string) : int",
                             AsCbMemberFn(&aiBRAIN::cbGetInt),
                             kEmptyErrHint, &this->sslFuncId_GetInt);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetString(desc : string) : string",
                             AsCbMemberFn(&aiBRAIN::cbGetString),
                             kEmptyErrHint, &this->sslFuncId_GetString);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetNameSpawnedFrom() : string",
                             AsCbMemberFn(&aiBRAIN::cbGetNameSpawnedFrom),
                             kEmptyErrHint, &this->sslFuncId_GetNameSpawnedFrom);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetViewUpCurve(curve : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetViewUpCurve),
                             kEmptyErrHint, &this->sslFuncId_SetViewUpCurve);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CalcReticleDanger() : float",
                             AsCbMemberFn(&aiBRAIN::cbCalcReticleDanger),
                             kEmptyErrHint, &this->sslFuncId_CalcReticleDanger);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetTimer(duration : float, isCycle : bool = false) : int",
                             AsCbMemberFn(&aiBRAIN::cbSetTimer),
                             kEmptyErrHint, &this->sslFuncId_SetTimer);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("KillTimer(timerID : int)",
                             AsCbMemberFn(&aiBRAIN::cbKillTimer),
                             kEmptyErrHint, &this->sslFuncId_KillTimer);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnTimer(timerID : int)",
                             AsCbMemberFn(&aiBRAIN::cbOnTimer),
                             kEmptyErrHint, &this->sslFuncId_OnTimer);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EnemyResetMovementDist()",
                             AsCbMemberFn(&aiBRAIN::cbEnemyResetMovementDist),
                             kEmptyErrHint, &this->sslFuncId_EnemyResetMovementDist);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EnemyGetMovementDist() : float",
                             AsCbMemberFn(&aiBRAIN::cbEnemyGetMovementDist),
                             kEmptyErrHint, &this->sslFuncId_EnemyGetMovementDist);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ThrowWeaponsAndItems()",
                             AsCbMemberFn(&aiBRAIN::cbThrowWeaponsAndItems),
                             kEmptyErrHint, &this->sslFuncId_ThrowWeaponsAndItems);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("LockEnemy(time : float)",
                             AsCbMemberFn(&aiBRAIN::cbLockEnemy),
                             kEmptyErrHint, &this->sslFuncId_LockEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SpareEnemy(time : float)",
                             AsCbMemberFn(&aiBRAIN::cbSpareEnemy),
                             kEmptyErrHint, &this->sslFuncId_SpareEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("Exclaim(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbExclaim),
                             kEmptyErrHint, &this->sslFuncId_Exclaim);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnExStarted(name : string, duration : float)",
                             AsCbMemberFn(&aiBRAIN::cbOnExStarted),
                             kEmptyErrHint, &this->sslFuncId_OnExStarted);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnExPlayed(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbOnExPlayed),
                             kEmptyErrHint, &this->sslFuncId_OnExPlayed);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnExCancelled(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbOnExCancelled),
                             kEmptyErrHint, &this->sslFuncId_OnExCancelled);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("FindExPartner(maxDist : float = 30) : object",
                             AsCbMemberFn(&aiBRAIN::cbFindExPartner),
                             kEmptyErrHint, &this->sslFuncId_FindExPartner);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetNavSys(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetNavSys),
                             kEmptyErrHint, &this->sslFuncId_SetNavSys);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetNavSys() : string",
                             AsCbMemberFn(&aiBRAIN::cbGetNavSys),
                             kEmptyErrHint, &this->sslFuncId_GetNavSys);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("MoveToPoint(wpName : string, bhvName : string = \"\", type : string = \"AUTO\", doPushBHV : bool = false) : bool",
                             AsCbMemberFn(&aiBRAIN::cbMoveToPoint),
                             kEmptyErrHint, &this->sslFuncId_MoveToPoint);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("MoveToHO(hoName : string, bhvName : string = \"\", type : string = \"AUTO\", doPushBHV : bool = false) : bool",
                             AsCbMemberFn(&aiBRAIN::cbMoveToHO),
                             kEmptyErrHint, &this->sslFuncId_MoveToHO);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("MoveToNavSys(nsName : string, bhvName : string = \"\", type : string = \"AUTO\", doPushBHV : bool = false) : bool",
                             AsCbMemberFn(&aiBRAIN::cbMoveToNavSys),
                             kEmptyErrHint, &this->sslFuncId_MoveToNavSys);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("HasHOInNavSys() : bool",
                             AsCbMemberFn(&aiBRAIN::cbHasHOInNavSys),
                             kEmptyErrHint, &this->sslFuncId_HasHOInNavSys);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("ForgetStuck()",
                             AsCbMemberFn(&aiBRAIN::cbForgetStuck),
                             kEmptyErrHint, &this->sslFuncId_ForgetStuck);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetDistToObj(obj: object) : float",
                             AsCbMemberFn(&aiBRAIN::cbGetDistToObj),
                             kEmptyErrHint, &this->sslFuncId_GetDistToObj);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetDistToWP(nameWP: string) : float",
                             AsCbMemberFn(&aiBRAIN::cbGetDistToWP),
                             kEmptyErrHint, &this->sslFuncId_GetDistToWP);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetClosestPoint(namesWPList: string, separator: string = \",\") : string",
                             AsCbMemberFn(&aiBRAIN::cbGetClosestPoint),
                             kEmptyErrHint, &this->sslFuncId_GetClosestPoint);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetPathRemain() : float",
                             AsCbMemberFn(&aiBRAIN::cbGetPathRemain),
                             kEmptyErrHint, &this->sslFuncId_GetPathRemain);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetDistPlayerToPathEnd() : float",
                             AsCbMemberFn(&aiBRAIN::cbGetDistPlayerToPathEnd),
                             kEmptyErrHint, &this->sslFuncId_GetDistPlayerToPathEnd);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetForceSpeed(speedName : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbSetForceSpeed),
                             kEmptyErrHint, &this->sslFuncId_SetForceSpeed);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndForceSpeed()",
                             AsCbMemberFn(&aiBRAIN::cbEndForceSpeed),
                             kEmptyErrHint, &this->sslFuncId_EndForceSpeed);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("UseSmartObject(smtObjName : string, bhvName : string = \"\", invite : string = \"\", doPushBHV : bool = false, typeSpeed : string = \"AUTO\") : bool",
                             AsCbMemberFn(&aiBRAIN::cbUseSmartObject),
                             kEmptyErrHint, &this->sslFuncId_UseSmartObject);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("UseSmartObjectInstant(smtObjName : string, bhvName : string = \"\", doPushBHV : bool = false) : bool",
                             AsCbMemberFn(&aiBRAIN::cbUseSmartObjectInstant),
                             kEmptyErrHint, &this->sslFuncId_UseSmartObjectInstant);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("AttachBodyToSmtObj(smtObjName : string, entryPointName : string = \"\") : bool",
                             AsCbMemberFn(&aiBRAIN::cbAttachBodyToSmtObj),
                             kEmptyErrHint, &this->sslFuncId_AttachBodyToSmtObj);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("UseTurret(smtObjName : string, bhvName : string = \"\", checkAngles : bool = true, doPushBHV : bool = false) : bool",
                             AsCbMemberFn(&aiBRAIN::cbUseTurret),
                             kEmptyErrHint, &this->sslFuncId_UseTurret);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("FindBestTurret(radius : float, isCheckAnges : bool) : string",
                             AsCbMemberFn(&aiBRAIN::cbFindBestTurret),
                             kEmptyErrHint, &this->sslFuncId_FindBestTurret);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("UseSmartIdleInstant(smtObjName : string, bhvName : string = \"\") : bool",
                             AsCbMemberFn(&aiBRAIN::cbUseSmartIdleInstant),
                             kEmptyErrHint, &this->sslFuncId_UseSmartIdleInstant);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetBHV(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetBHV),
                             kEmptyErrHint, &this->sslFuncId_SetBHV);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("PushBHV(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbPushBHV),
                             kEmptyErrHint, &this->sslFuncId_PushBHV);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("PushSHOOT(bhvName : string = \"\") : bool",
                             AsCbMemberFn(&aiBRAIN::cbPushSHOOT),
                             kEmptyErrHint, &this->sslFuncId_PushSHOOT);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetBHV() : string",
                             AsCbMemberFn(&aiBRAIN::cbGetBHV),
                             kEmptyErrHint, &this->sslFuncId_GetBHV);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetMajorBHVType() : string",
                             AsCbMemberFn(&aiBRAIN::cbGetMajorBHVType),
                             kEmptyErrHint, &this->sslFuncId_GetMajorBHVType);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetBHVOnEmptyStack() : bool",
                             AsCbMemberFn(&aiBRAIN::cbSetBHVOnEmptyStack),
                             kEmptyErrHint, &this->sslFuncId_SetBHVOnEmptyStack);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CallBhvFuncOnce(bhvPrefix: string, nameFunc: string)",
                             AsCbMemberFn(&aiBRAIN::cbCallBhvFuncOnce),
                             kEmptyErrHint, &this->sslFuncId_CallBhvFuncOnce);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CancelBHVByNameAndStackAbove(bhvName : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbCancelBHVByNameAndStackAbove),
                             kEmptyErrHint, &this->sslFuncId_CancelBHVByNameAndStackAbove);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("FailBHVByNameAndStackAbove(bhvName : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbFailBHVByNameAndStackAbove),
                             kEmptyErrHint, &this->sslFuncId_FailBHVByNameAndStackAbove);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("JoinGroup(typeName : string, name : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbJoinGroup),
                             kEmptyErrHint, &this->sslFuncId_JoinGroup);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("LeaveGroup()",
                             AsCbMemberFn(&aiBRAIN::cbLeaveGroup),
                             kEmptyErrHint, &this->sslFuncId_LeaveGroup);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("HaveGroup() : bool",
                             AsCbMemberFn(&aiBRAIN::cbHaveGroup),
                             kEmptyErrHint, &this->sslFuncId_HaveGroup);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetGroup() : object",
                             AsCbMemberFn(&aiBRAIN::cbGetGroup),
                             kEmptyErrHint, &this->sslFuncId_GetGroup);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetInterest(interestPoint : string, interestMode : string, disableIdling : bool = true)",
                             AsCbMemberFn(&aiBRAIN::cbSetInterest),
                             kEmptyErrHint, &this->sslFuncId_SetInterest);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("EndInterest()",
                             AsCbMemberFn(&aiBRAIN::cbEndInterest),
                             kEmptyErrHint, &this->sslFuncId_EndInterest);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetTeamName() : string",
                             AsCbMemberFn(&aiBRAIN::cbGetTeamName),
                             kEmptyErrHint, &this->sslFuncId_GetTeamName);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("SetTeamName(name : string)",
                             AsCbMemberFn(&aiBRAIN::cbSetTeamName),
                             kEmptyErrHint, &this->sslFuncId_SetTeamName);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CountMyNeutralsAroundMe(radius : float) : int",
                             AsCbMemberFn(&aiBRAIN::cbCountMyNeutralsAroundMe),
                             kEmptyErrHint, &this->sslFuncId_CountMyNeutralsAroundMe);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("CountMyNeutralsAroundEnemy(radius : float) : int",
                             AsCbMemberFn(&aiBRAIN::cbCountMyNeutralsAroundEnemy),
                             kEmptyErrHint, &this->sslFuncId_CountMyNeutralsAroundEnemy);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tDo(par1 : *, par2 : * = null, par3 : * = null, par4 : * = null, par5 : * = null, par6 : * = null, par7 : * = null, par8 : * = null, par9 : * = null, par10 : * = null, par11 : * = null, par12 : * = null) : bool",
                             AsCbMemberFn(&aiBRAIN::cbtDo),
                             kEmptyErrHint, &this->sslFuncId_tDo);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tDoWait(par1 : *, par2 : * = null, par3 : * = null, par4 : * = null, par5 : * = null, par6 : * = null, par7 : * = null, par8 : * = null, par9 : * = null, par10 : * = null, par11 : * = null, par12 : * = null) : bool",
                             AsCbMemberFn(&aiBRAIN::cbtDoWait),
                             kEmptyErrHint, &this->sslFuncId_tDoWait);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tNDo(obj : string, fn : string, par1 : * = null, par2 : * = null, par3 : * = null, par4 : * = null, par5 : * = null, par6 : * = null, par7 : * = null, par8 : * = null, par9 : * = null, par10 : * = null) : bool",
                             AsCbMemberFn(&aiBRAIN::cbtNDo),
                             kEmptyErrHint, &this->sslFuncId_tNDo);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tNDoWait(obj : string, fn : string, par1 : * = null, par2 : * = null, par3 : * = null, par4 : * = null, par5 : * = null, par6 : * = null, par7 : * = null, par8 : * = null, par9 : * = null, par10 : * = null) : bool",
                             AsCbMemberFn(&aiBRAIN::cbtNDoWait),
                             kEmptyErrHint, &this->sslFuncId_tNDoWait);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tClear()",
                             AsCbMemberFn(&aiBRAIN::cbtClear),
                             kEmptyErrHint, &this->sslFuncId_tClear);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tWaitBHV(bhvName : string)",
                             AsCbMemberFn(&aiBRAIN::cbtWaitBHV),
                             kEmptyErrHint, &this->sslFuncId_tWaitBHV);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tWait(par1 : *)",
                             AsCbMemberFn(&aiBRAIN::cbtWait),
                             kEmptyErrHint, &this->sslFuncId_tWait);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tLock(count : int = 1)",
                             AsCbMemberFn(&aiBRAIN::cbtLock),
                             kEmptyErrHint, &this->sslFuncId_tLock);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tUnlock(count : int = 1)",
                             AsCbMemberFn(&aiBRAIN::cbtUnlock),
                             kEmptyErrHint, &this->sslFuncId_tUnlock);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tSetAFCount(count : int)",
                             AsCbMemberFn(&aiBRAIN::cbtSetAFCount),
                             kEmptyErrHint, &this->sslFuncId_tSetAFCount);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tSetAFDelay(delayTime : float)",
                             AsCbMemberFn(&aiBRAIN::cbtSetAFDelay),
                             kEmptyErrHint, &this->sslFuncId_tSetAFDelay);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("tEnableForceAF(enable : bool)",
                             AsCbMemberFn(&aiBRAIN::cbtEnableForceAF),
                             kEmptyErrHint, &this->sslFuncId_tEnableForceAF);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("TestParam2AI(p : params)",
                             AsCbMemberFn(&aiBRAIN::cbTestParam2AI),
                             kEmptyErrHint, &this->sslFuncId_TestParam2AI);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("TestParamFromAI() : params",
                             AsCbMemberFn(&aiBRAIN::cbTestParamFromAI),
                             kEmptyErrHint, &this->sslFuncId_TestParamFromAI);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_Reset()",
                             AsCbMemberFn(&aiBRAIN::cbQOT_Reset),
                             kEmptyErrHint, &this->sslFuncId_QOT_Reset);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_Put(p : params)",
                             AsCbMemberFn(&aiBRAIN::cbQOT_Put),
                             kEmptyErrHint, &this->sslFuncId_QOT_Put);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_GetFront() : params",
                             AsCbMemberFn(&aiBRAIN::cbQOT_GetFront),
                             kEmptyErrHint, &this->sslFuncId_QOT_GetFront);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_ClearFront()",
                             AsCbMemberFn(&aiBRAIN::cbQOT_ClearFront),
                             kEmptyErrHint, &this->sslFuncId_QOT_ClearFront);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_Size() : int",
                             AsCbMemberFn(&aiBRAIN::cbQOT_Size),
                             kEmptyErrHint, &this->sslFuncId_QOT_Size);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("QOT_IsEmpty() : bool",
                             AsCbMemberFn(&aiBRAIN::cbQOT_IsEmpty),
                             kEmptyErrHint, &this->sslFuncId_QOT_IsEmpty);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("AvoidDanger(reactions : string) : bool",
                             AsCbMemberFn(&aiBRAIN::cbAvoidDanger),
                             kEmptyErrHint, &this->sslFuncId_AvoidDanger);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("AvoidGrenade(mpID : int, reactions : string = \"FallDown,RollAway,RollAwayAndWait\", delayToStart : float = 0.0) : bool",
                             AsCbMemberFn(&aiBRAIN::cbAvoidGrenade),
                             kEmptyErrHint, &this->sslFuncId_AvoidGrenade);
    if (err.id)
        return 0;
    return 1;
}
