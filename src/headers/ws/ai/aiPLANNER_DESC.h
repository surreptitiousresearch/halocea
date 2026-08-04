#pragma once
#include "../ia/iaDESC.h"
#include "../DEFAULT_CTOR.h"
#include "../ps/psSECTION.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: descriptor (parsed config template) for the global aiPLANNER. Extends iaDESC
// with the cache of SSL function ids the planner exports. DB-verified layout
// (types_members aiPLANNER_DESC) — size 0x128: iaDESC@0 (0xA4, base), isSslClassInited@0xA4,
// then 29 int SSL-function-id fields (sslFuncId_NavLink @0xA8 .. sslFuncId_tIsFlag @0x124).

struct aiPLANNER_DESC : iaDESC {
    DEFAULT_CTOR<bool> isSslClassInited; // 0xA4
    unsigned char      _padA5[3];        // 0xA5
    int sslFuncId_NavLink;               // 0xA8
    int sslFuncId_NavLinkSingle;         // 0xAC
    int sslFuncId_NavUnLink;             // 0xB0
    int sslFuncId_NavUnLinkSingle;       // 0xB4
    int sslFuncId_NavBlockObj;           // 0xB8
    int sslFuncId_NavUnBlockObj;         // 0xBC
    int sslFuncId_NavDisableWP;          // 0xC0
    int sslFuncId_NavEnableWP;           // 0xC4
    int sslFuncId_NavForceAutolinkWP;    // 0xC8
    int sslFuncId_NavRelinkWPToNS;       // 0xCC
    int sslFuncId_NavSysMarkStatic;      // 0xD0
    int sslFuncId_NavSysPenalizeByDomain;// 0xD4
    int sslFuncId_NavSysRemove;          // 0xD8
    int sslFuncId_SetTeamsEnemies;       // 0xDC
    int sslFuncId_SetTeamsNeutrals;      // 0xE0
    int sslFuncId_IsFriendly;            // 0xE4
    int sslFuncId_SetHOMaxAngleCorner;   // 0xE8
    int sslFuncId_SetHOMaxAngleMiddle;   // 0xEC
    int sslFuncId_FindGroupByName;       // 0xF0
    int sslFuncId_CountLiveInDomain;     // 0xF4
    int sslFuncId_NoticeFlyingGrenade;   // 0xF8
    int sslFuncId_NoticeGrenadeExplosion;// 0xFC
    int sslFuncId_NoticeFlyingRocket;    // 0x100
    int sslFuncId_RegisterDeathSmtObject;// 0x104
    int sslFuncId_UnregisterDeathSmtObject;// 0x108
    int sslFuncId_EnableStrike;          // 0x10C
    int sslFuncId_DisableStrike;         // 0x110
    int sslFuncId_AddEnemy;              // 0x114
    int sslFuncId_RemoveEnemy;           // 0x118
    int sslFuncId_tSetFlag;              // 0x11C
    int sslFuncId_tClearFlag;            // 0x120
    int sslFuncId_tIsFlag;               // 0x124

    // 0x83163A08 (virtual) — parse the shared iaDESC config, then force priority 44.
    // REVERSED: src/ws/ai/aiPLANNER_DESC__ParsePS.cpp.
    void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);

    // 0x8325AA50 (?InitSslClass@aiPLANNER_DESC@@UAAHABV?$dsTSTRING@D@@@Z, virtual) — chain up to the
    // iaDESC base init, then (once, guarded by isSslClassInited) unshare this class's sslClass ref and
    // register every aiPLANNER "Nav*/team/flag/enemy/..." SSL callback on it, short-circuiting on the
    // first registration error. Returns 1 on success, 0 on failure.
    // REVERSED: src/ws/ai/aiPLANNER_DESC__InitSslClass.cpp.
    int InitSslClass(const dsTSTRING<char> &hintErr);
};
