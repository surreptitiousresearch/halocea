#pragma once
#include "../ssl/sslDESC.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ps/psSECTION.h"
// ws-engine prop: descriptor base for propBASE-derived properties — the parsed, shared config
// template a live propBASE instance's spDesc points at (name, sslClass link, per-instance
// script text). iaDESC (pOwnerDesc) is the owning entity's descriptor.
// DB-verified layout (types_members propBASE_DESC): <sslDESC base>@0 (20), pOwnerDesc@20,
// sslState@24, sslScripts@28 (20), name@48 — size 52.

struct iaDESC; // ws-engine actor descriptor — opaque boundary, not decompiled here

// This class's own vtable slot (propHALO_OBJ_DESC::ParsePS/etc. reset __vftable back to this on
// destruction). Opaque — not expanded; only its address is taken. // boundary
struct propBASE_DESC_vtbl;

struct propBASE_DESC : sslDESC {
    iaDESC               *pOwnerDesc;  // 0x14 owning entity's descriptor
    apSTATE_T<unsigned char> sslState; // 0x18 script attach/init state
    unsigned char _pad0[3]; /* db-verified padding */
    dsVECTOR<dsTSTRING<char>, 8> sslScripts; // 0x1C per-instance SSL script source list
    dsSTRID                name;       // 0x30

    static propBASE_DESC_vtbl vftable; // `propBASE_DESC::`vftable'' — boundary, not decompiled

    // 0x823EAF40 — default construct. Body external to this batch. // boundary
    propBASE_DESC();
    // 0x823EB050 (virtual) — destruct. Body external to this batch. // boundary
    virtual ~propBASE_DESC();
    // 0x823E06C8 — memberwise copy-assign. Body external to this batch. // boundary
    propBASE_DESC &operator=(const propBASE_DESC &that);

    // 0x826C4868 (?InitSslClass@propBASE_DESC@@UAAHABV?$dsTSTRING@D@@@Z) — virtual; called by
    // propHALO_OBJ_DESC::InitSslClass before its own per-event registration. Body not decompiled in
    // this batch. // boundary
    virtual int InitSslClass(const dsTSTRING<char> &hintErr);

    // 0x826C2438 (?UnshareSslClass@propBASE_DESC@@UAAX_N0@Z) — virtual override of
    // sslDESC::UnshareSslClass; called (via the base vtable slot) from propHALO_OBJ_DESC's
    // InitSslClass as `this->UnshareSslClass(false, false)`. Body external to this batch.
    // boundary
    virtual void UnshareSslClass(bool isBySslScript, bool isForceUnshare);

    // 0x826C8400 (?ParsePS@propBASE_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — virtual;
    // propHALO_OBJ_DESC::ParsePS delegates to this first. psSECTION passed by value (invisible
    // reference — see dscDESC_vtbl note). Body external to this batch. // boundary
    virtual void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);
};
