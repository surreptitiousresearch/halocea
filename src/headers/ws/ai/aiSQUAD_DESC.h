#pragma once
#include "../ent/entDESC.h"
#include "../DEFAULT_CTOR.h"

// ws-engine ai08 — per-class descriptor for aiSQUAD (the template data shared by all squads of a
// class). DB-verified layout (types_members aiSQUAD_DESC) — size 472 (0x1D8):
//   entDESC@0 (440B base), isSslClassInited@0x1B8, then the per-class SSL *function* ids resolved
//   at class init. Each sslFuncId_* is the id that aiSQUAD's SSL-dispatch methods feed to
//   sslObject.CallFunc (see aiSQUAD::OnCreate / GetGroup / IsMemberInFront / GetDistMemberToFront...).

struct aiSQUAD_DESC : entDESC {
    DEFAULT_CTOR<bool> isSslClassInited;             // 0x1B8
    int sslFuncId_OnCreate;                          // 0x1BC
    int sslFuncId_OnDbgPause;                         // 0x1C0
    int sslFuncId_OnDbgResume;                        // 0x1C4
    int sslFuncId_GetGroup;                          // 0x1C8
    int sslFuncId_GetDistMemberToFront;              // 0x1CC
    int sslFuncId_GetDistMemberToFrontTarget;        // 0x1D0
    int sslFuncId_IsMemberInFront;                   // 0x1D4

    // 0x832A7F68 (?ParsePS@aiSQUAD_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — virtual: parse the
    // squad-class descriptor: bump the descriptor priority to 70, then defer to entDESC. REVERSED.
    void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);

    // 0x832A9A78 (?InitSslClass@aiSQUAD_DESC@@UAAHABV?$dsTSTRING@D@@@Z) — one-time SSL-class setup:
    // chain to entDESC::InitSslClass, then (guarded by isSslClassInited) unshare this class's
    // sslClass and register the squad's seven native callback functions on it, recording each id
    // into the sslFuncId_ tail. REVERSED: src/ws/ai/aiSQUAD_DESC__InitSslClass.cpp.
    int InitSslClass(const dsTSTRING<char> &hintErr);

    // Give this descriptor its own (non-shared) sslClass before registering callbacks on it.
    // Body external to this batch. boundary.
    void UnshareSslClass(bool a, bool b);
};
