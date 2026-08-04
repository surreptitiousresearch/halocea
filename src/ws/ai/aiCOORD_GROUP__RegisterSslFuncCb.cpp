#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h" // complete sslCB_HOST for the sslCB_MEMBER_FN cast target
#include "../../headers/ws/ssl/sslERROR.h"

// aiCOORD_GROUP::RegisterSslFuncCb @ 0x83232750
// ?RegisterSslFuncCb@aiCOORD_GROUP@@SA_NAAVsslCLASS_REF@@@Z
//
// Static registration entry point: bind every aiCOORD_GROUP SSL callback as a script-callable
// native function on `sslClass`, using the member-callback overload sslCLASS_REF::AddCbFunc(decl,
// cbFunc, errHint, idx). The binary unrolls the 69 registrations inline; each call assigns the
// returned sslERROR into a single accumulator and bails out (returning false) on the first error.
// This is reproduced faithfully as a table walk with early-out — behaviour-identical to the
// unrolled form (each AddCbFunc's sret sslERROR is a per-iteration temporary that releases itself).
//
// The callbacks are pointer-to-members of aiCOORD_GROUP; AddCbFunc takes an sslCB_HOST::* (the host
// base of the coordinated-group object). The reinterpret_cast between pointer-to-member-function
// types matches the binary's cast at each call site.

extern char byte_8200155A[]; // shared empty string literal — passed as the errHint source-location

bool aiCOORD_GROUP::RegisterSslFuncCb(sslCLASS_REF &sslClass)
{
    // Native member-pointer type of the callbacks (their real class). The cast to the sslCB_HOST::*
    // slot type is done at the AddCbFunc call site — a reinterpret_cast between member pointers is
    // not a constant expression, so it cannot live in the static table's initializer.
    typedef void (aiCOORD_GROUP::*AiCgCb)(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    struct CbEntry {
        const char *decl;
        AiCgCb      cbFunc;
    };

    #define AICG_CB(m) (&aiCOORD_GROUP::m)

    static const CbEntry entries[] = {
        { "GetName() : string",                                              AICG_CB(cbGetName) },
        { "GetType() : string",                                              AICG_CB(cbGetType) },
        { "AddMember(ai : object) : bool",                                   AICG_CB(cbAddMember) },
        { "RemoveMember(ai : object)",                                       AICG_CB(cbRemoveMember) },
        { "LeadGroup(ai : object) : bool",                                   AICG_CB(cbLeadGroup) },
        { "HaveMembers() : bool",                                            AICG_CB(cbHaveMembers) },
        { "GetMembersCount() : int",                                         AICG_CB(cbGetMembersCount) },
        { "GetMember(idx : int) : object",                                   AICG_CB(cbGetMember) },
        { "GetMemberSorted(idx : int) : object",                             AICG_CB(cbGetMemberSorted) },
        { "GetLeader() : object",                                            AICG_CB(cbGetLeader) },
        { "IsContain(ai : object) : bool",                                   AICG_CB(cbIsContain) },
        { "IsGroupLeader(ai : object) : bool",                               AICG_CB(cbIsGroupLeader) },
        { "DestroyGroup()",                                                  AICG_CB(cbDestroyGroup) },
        { "SetTarget(name : string) : bool",                                 AICG_CB(cbSetTarget) },
        { "GetTarget() : object",                                            AICG_CB(cbGetTarget) },
        { "IsTargetValid() : bool",                                          AICG_CB(cbIsTargetValid) },
        { "IsTargetInHO() : bool",                                           AICG_CB(cbIsTargetInHO) },
        { "GetTimeTargetInHO() : float",                                     AICG_CB(cbGetTimeTargetInHO) },
        { "ChangeNSToTarget(numAssoc : int) : bool",                         AICG_CB(cbChangeNSToTarget) },
        { "AddNSToIgnoreList(nsName: string) : bool",                        AICG_CB(cbAddNSToIgnoreList) },
        { "RemoveNSFromIgnoreList(nsName: string) : bool",                   AICG_CB(cbRemoveNSFromIgnoreList) },
        { "ClearNSIgnoreList()",                                             AICG_CB(cbClearNSIgnoreList) },
        { "Is3DNavSys(ai : object = null) : bool",                           AICG_CB(cbIs3DNavSys) },
        { "StoreTargetPosForDeltaCalc()",                                    AICG_CB(cbStoreTargetPosForDeltaCalc) },
        { "GetTargetMovementDeltaDist() : float",                            AICG_CB(cbGetTargetMovementDeltaDist) },
        { "GetTargetMovementDeltaDistViaNavSys() : float",                   AICG_CB(cbGetTargetMovementDeltaDistViaNavSys) },
        { "GetDistToTargetDirect(ai : object) : float",                      AICG_CB(cbGetDistToTargetDirect) },
        { "GetDistToTargetViaNavSys(ai : object) : float",                   AICG_CB(cbGetDistToTargetViaNavSys) },
        { "SetDistFnCurve(curve : string)",                                  AICG_CB(cbSetDistFnCurve) },
        { "SetAngleFnCurve(curve : string)",                                 AICG_CB(cbSetAngleFnCurve) },
        { "IsEqualCurves(curve1 : string, curve2 : string, precision : float = 10) : bool", AICG_CB(cbIsEqualCurves) },
        { "IsValidFront(front : string) : bool",                             AICG_CB(cbIsValidFront) },
        { "IsNeedFrontUpdate(front : string) : bool",                        AICG_CB(cbIsNeedFrontUpdate) },
        { "ClearWPSet()",                                                    AICG_CB(cbClearWPSet) },
        { "HaveWPSet() : bool",                                              AICG_CB(cbHaveWPSet) },
        { "GetWPSetCount() : int",                                           AICG_CB(cbGetWPSetCount) },
        { "EnableReuseWPSet()",                                              AICG_CB(cbEnableReuseWPSet) },
        { "DisableReuseWPSet()",                                             AICG_CB(cbDisableReuseWPSet) },
        { "EnsureHaveWPSet() : bool",                                        AICG_CB(cbEnsureHaveWPSet) },
        { "IsTargetWPInWPSet(ai : object) : bool",                           AICG_CB(cbIsTargetWPInWPSet) },
        { "AddFrontToExcluders(front : string) : bool",                      AICG_CB(cbAddFrontToExcluders) },
        { "RemoveFrontFromExcluders(front : string) : bool",                 AICG_CB(cbRemoveFrontFromExcluders) },
        { "ClearExcluders()",                                                AICG_CB(cbClearExcluders) },
        { "GetWPSetFromNavSys() : bool",                                     AICG_CB(cbGetWPSetFromNavSys) },
        { "GetWPSetFromFront(front : string) : bool",                        AICG_CB(cbGetWPSetFromFront) },
        { "GetWPSetFromTargetAssocDistFn(count : int, assocHOShift : bool, wpAssocShift : float = 0) : bool", AICG_CB(cbGetWPSetFromTargetAssocDistFn) },
        { "GetWPSetFromTargetDistFn(count : int) : bool",                    AICG_CB(cbGetWPSetFromTargetDistFn) },
        { "GetWPSetHOFromTargetDistFn(count : int) : bool",                  AICG_CB(cbGetWPSetHOFromTargetDistFn) },
        { "GetWPSetFromChaser(ai : object) : bool",                          AICG_CB(cbGetWPSetFromChaser) },
        { "FilterWPSetByFront(front : string) : bool",                       AICG_CB(cbFilterWPSetByFront) },
        { "FilterWPSetByTargetAssocDistFn(count : int, assocHOShift : bool, wpAssocShift : float = 0) : bool", AICG_CB(cbFilterWPSetByTargetAssocDistFn) },
        { "FilterWPSetByTargetDistFn(count : int = -1) : bool",              AICG_CB(cbFilterWPSetByTargetDistFn) },
        { "FilterWPSetByTargetAngleFn() : bool",                             AICG_CB(cbFilterWPSetByTargetAngleFn) },
        { "FilterWPSetByMaxDistToTarget(maxDist : float) : bool",            AICG_CB(cbFilterWPSetByMaxDistToTarget) },
        { "FilterWPSetByDistToEnemy(ai : object, countPoints : int) : bool", AICG_CB(cbFilterWPSetByDistToEnemy) },
        { "FilterWPSetByPeerPenalty(ai : object) : bool",                    AICG_CB(cbFilterWPSetByPeerPenalty) },
        { "FilterWPSetByHO(checkCanAcquireHO : bool = true) : bool",         AICG_CB(cbFilterWPSetByHO) },
        { "FilterWPSetByHOvsEnemy(ai : object, minDistToEnemy : float) : bool", AICG_CB(cbFilterWPSetByHOvsEnemy) },
        { "FilterWPSetByHOvsTargetHODir() : bool",                           AICG_CB(cbFilterWPSetByHOvsTargetHODir) },
        { "FilterWPSetByAngleVsTarget(ai : object, maxAbsAngle : float) : bool", AICG_CB(cbFilterWPSetByAngleVsTarget) },
        { "FilterWPSetByDistToEnemyLessDist(ai : object, distDelta : float) : bool", AICG_CB(cbFilterWPSetByDistToEnemyLessDist) },
        { "FilterWPSetByDistToEnemySameDist(ai : object, distDelta : float) : bool", AICG_CB(cbFilterWPSetByDistToEnemySameDist) },
        { "FilterWPSetByDistFromBrain(ai : object, radiusExclude : float) : bool", AICG_CB(cbFilterWPSetByDistFromBrain) },
        { "FilterWPSetByDistFromUsedWPs(selfAI : object, radiusExclude : float) : bool", AICG_CB(cbFilterWPSetByDistFromUsedWPs) },
        { "FilterWPSetByMinHeight(heightDelta : float) : bool",              AICG_CB(cbFilterWPSetByMinHeight) },
        { "FilterWPSetByTargetHOPlane() : bool",                            AICG_CB(cbFilterWPSetByTargetHOPlane) },
        { "GetSquadActor(squadActorName : string) : object",                 AICG_CB(cbGetSquadActor) },
        { "SmokeOutEnemyFromHO()",                                           AICG_CB(cbSmokeOutEnemyFromHO) },
        { "AddPossibleEnemiesFromTarget(ai : object)",                       AICG_CB(cbAddPossibleEnemiesFromTarget) },
    };

    #undef AICG_CB

    const int count = static_cast<int>(sizeof(entries) / sizeof(entries[0]));
    for (int i = 0; i < count; ++i)
    {
        sslERROR err = sslClass.AddCbFunc(entries[i].decl,
            reinterpret_cast<sslCB_MEMBER_FN>(entries[i].cbFunc), byte_8200155A, nullptr);
        if (err.id != SSL_ERR_NO_ERROR)
            return false;
    }
    return true;
}
