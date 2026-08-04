#include "../../headers/ws/ai/aiPLANNER_DESC.h"
#include "../../headers/ws/ai/aiPLANNER.h"       // aiPLANNER::cb* callbacks registered below
#include "../../headers/ws/ia/iaDESC.h"          // iaDESC_vtbl (virtual UnshareSslClass slot)
#include "../../headers/ws/ssl/sslCLASS_REF.h"   // AddCbFunc (member overload) + sslCB_MEMBER_FN
#include "../../headers/ws/ssl/sslCB_HOST.h"     // sslCB_MEMBER_FN target host type
#include "../../headers/ws/ssl/sslERROR.h"       // sslERROR / SSL_ERR_NO_ERROR

// Base iaDESC concrete InitSslClass implementation, invoked non-virtually (direct bl at 0x8325AA60).
// 0x826B2EC8 (?InitSslClass@iaDESC@@UAAHABV?$dsTSTRING@D@@@Z). Declared as a free-function shim because
// iaDESC models its vtable as a plain function-pointer table member named InitSslClass (so the method
// name is not available on the type). boundary.
extern "C++" int aiPLANNER_DESC_InitSslClass_iaDESC_base(iaDESC *self, const dsTSTRING<char> &hintErr);

// aiPLANNER_DESC::InitSslClass(const dsTSTRING<char>&) @ 0x8325AA50
// ?InitSslClass@aiPLANNER_DESC@@UAAHABV?$dsTSTRING@D@@@Z (virtual)
//
// Chain up to the iaDESC base init (bail out on failure). Then, only once (guarded by
// isSslClassInited), unshare this class's sslClass ref and register every aiPLANNER SSL callback
// (Nav*/team/HO/notice/death-smt/group/domain/strike/enemy/flag) on it, short-circuiting on the first
// registration error.
//
// The member-form sslCLASS_REF::AddCbFunc takes a pointer-to-member callback typed against sslCB_HOST;
// each aiPLANNER::cb* is reinterpret_cast to that type exactly as the compiler-emitted cast at each
// call site does. The errHint passed to every call is the fixed empty-string rodata byte
// (byte_8200155A), modeled here as a shared empty C string literal. RAII: each AddCbFunc's returned
// sslERROR temporary self-releases (the assign-then-destroy pair the disassembly inlines after every
// call).
int aiPLANNER_DESC::InitSslClass(const dsTSTRING<char> &hintErr)
{
    static const char *const kEmptyErrHint = ""; // byte_8200155A

    if (!aiPLANNER_DESC_InitSslClass_iaDESC_base(this, hintErr))
        return 0;

    if (this->isSslClassInited.val)
        return 1;

    this->isSslClassInited.val = true;

    // Virtual UnshareSslClass (vtable slot 0x1C); __vftable's static type is dscDESC_vtbl* — reinterpret
    // to the wider iaDESC_vtbl to reach the slot.
    reinterpret_cast<const iaDESC_vtbl *>(this->__vftable)->UnshareSslClass(this, false, false);

    sslERROR err;

    #define AI_PLANNER_ADD_CB(decl, method, idField)                                             \
        err = this->sslClass.AddCbFunc((decl),                                                    \
                  reinterpret_cast<sslCB_MEMBER_FN>(&aiPLANNER::method),                          \
                  kEmptyErrHint, &this->idField);                                                 \
        if (err.id)                                                                               \
            return 0;

    AI_PLANNER_ADD_CB("NavLink(sys : string, wpA : string, wpB : string, smtObjName : string = \"\")",
                      cbNavLink, sslFuncId_NavLink)
    AI_PLANNER_ADD_CB("NavLinkSingle(sys : string, wpA : string, wpB : string, smtObjName : string = \"\")",
                      cbNavLinkSingle, sslFuncId_NavLinkSingle)
    AI_PLANNER_ADD_CB("NavUnLink(sys : string, wpA : string, wpB : string)",
                      cbNavUnLink, sslFuncId_NavUnLink)
    AI_PLANNER_ADD_CB("NavUnLinkSingle(sys : string, wpA : string, wpB : string)",
                      cbNavUnLinkSingle, sslFuncId_NavUnLinkSingle)
    AI_PLANNER_ADD_CB("NavBlockObj(instName : string, objName : string = \"\") : bool",
                      cbNavBlockObj, sslFuncId_NavBlockObj)
    AI_PLANNER_ADD_CB("NavUnBlockObj(instName : string, objName : string = \"\") : bool",
                      cbNavUnBlockObj, sslFuncId_NavUnBlockObj)
    AI_PLANNER_ADD_CB("NavDisableWP(wpName : string) : bool",
                      cbNavDisableWP, sslFuncId_NavDisableWP)
    AI_PLANNER_ADD_CB("NavEnableWP(wpName : string) : bool",
                      cbNavEnableWP, sslFuncId_NavEnableWP)
    AI_PLANNER_ADD_CB("NavForceAutolinkWP(wpName : string) : bool",
                      cbNavForceAutolinkWP, sslFuncId_NavForceAutolinkWP)
    AI_PLANNER_ADD_CB("NavRelinkWPToNS(wpName : string, sysName : string) : bool",
                      cbNavRelinkWPToNS, sslFuncId_NavRelinkWPToNS)
    AI_PLANNER_ADD_CB("NavSysMarkStatic(nsName : string, isStatic : bool) : bool",
                      cbNavSysMarkStatic, sslFuncId_NavSysMarkStatic)
    AI_PLANNER_ADD_CB("NavSysPenalizeByDomain(sys : string, dom : string, level : int) : bool",
                      cbNavSysPenalizeByDomain, sslFuncId_NavSysPenalizeByDomain)
    AI_PLANNER_ADD_CB("NavSysRemove(nsName : string) : bool",
                      cbNavSysRemove, sslFuncId_NavSysRemove)
    AI_PLANNER_ADD_CB("SetTeamsEnemies(teamFrom : string, teamTo : string) : bool",
                      cbSetTeamsEnemies, sslFuncId_SetTeamsEnemies)
    AI_PLANNER_ADD_CB("SetTeamsNeutrals(teamFrom : string, teamTo : string) : bool",
                      cbSetTeamsNeutrals, sslFuncId_SetTeamsNeutrals)
    AI_PLANNER_ADD_CB("IsFriendly(obj1 : object, obj2 : object) : bool",
                      cbIsFriendly, sslFuncId_IsFriendly)
    AI_PLANNER_ADD_CB("SetHOMaxAngleCorner(maxAngleCorner : float)",
                      cbSetHOMaxAngleCorner, sslFuncId_SetHOMaxAngleCorner)
    AI_PLANNER_ADD_CB("SetHOMaxAngleMiddle(maxAngleMiddle : float)",
                      cbSetHOMaxAngleMiddle, sslFuncId_SetHOMaxAngleMiddle)
    AI_PLANNER_ADD_CB("NoticeFlyingGrenade(obj1 : object)",
                      cbNoticeFlyingGrenade, sslFuncId_NoticeFlyingGrenade)
    AI_PLANNER_ADD_CB("NoticeGrenadeExplosion(obj1 : object)",
                      cbNoticeGrenadeExplosion, sslFuncId_NoticeGrenadeExplosion)
    AI_PLANNER_ADD_CB("NoticeFlyingRocket(obj1 : object)",
                      cbNoticeFlyingRocket, sslFuncId_NoticeFlyingRocket)
    AI_PLANNER_ADD_CB("RegisterDeathSmtObject(smtObj : object)",
                      cbRegisterDeathSmtObject, sslFuncId_RegisterDeathSmtObject)
    AI_PLANNER_ADD_CB("UnregisterDeathSmtObject(smtObj : object)",
                      cbUnregisterDeathSmtObject, sslFuncId_UnregisterDeathSmtObject)
    AI_PLANNER_ADD_CB("FindGroupByName(grpName : string) : object",
                      cbFindGroupByName, sslFuncId_FindGroupByName)
    AI_PLANNER_ADD_CB("CountLiveInDomain(dom : object, team : string = \"\") : int",
                      cbCountLiveInDomain, sslFuncId_CountLiveInDomain)
    AI_PLANNER_ADD_CB("EnableStrike(nameStrike : string)",
                      cbEnableStrike, sslFuncId_EnableStrike)
    AI_PLANNER_ADD_CB("DisableStrike(nameStrike : string)",
                      cbDisableStrike, sslFuncId_DisableStrike)
    AI_PLANNER_ADD_CB("AddEnemy(obj: object): bool",
                      cbAddEnemy, sslFuncId_AddEnemy)
    AI_PLANNER_ADD_CB("RemoveEnemy(obj: object)",
                      cbRemoveEnemy, sslFuncId_RemoveEnemy)
    AI_PLANNER_ADD_CB("tSetFlag(flag : string) : bool",
                      cbtSetFlag, sslFuncId_tSetFlag)
    AI_PLANNER_ADD_CB("tClearFlag(flag : string) : bool",
                      cbtClearFlag, sslFuncId_tClearFlag)

    #undef AI_PLANNER_ADD_CB

    // Final registration (tIsFlag): success iff it returned no error.
    err = this->sslClass.AddCbFunc("tIsFlag(flag : string) : bool",
              reinterpret_cast<sslCB_MEMBER_FN>(&aiPLANNER::cbtIsFlag),
              kEmptyErrHint, &this->sslFuncId_tIsFlag);

    return err.id == SSL_ERR_NO_ERROR;
}
