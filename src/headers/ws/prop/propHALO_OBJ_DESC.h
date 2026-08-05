#pragma once
#include "propENT_DESC.h"
#include "../DEFAULT_CTOR.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsTYPE_ID.h"
#include "../ds/dsTSTRING.h"
#include "../ps/psSECTION.h"
// ws-engine prop: descriptor for propHALO_OBJ — carries the SSL event-id bindings for the four
// in/out change-event pairs plus the configured object-use state and termination-time policy.
// DB-verified layout (types_members propHALO_OBJ_DESC): <propENT_DESC base>@0 (52),
// isEntSslClassInited@52, sslEventId_OnChangeInA..D@56/60/64/68, sslEventId_OnChangeOutA..D@72/
// 76/80/84, objUse@88, timeTerm@92 — size 96.

// This class's own vtable slot. Opaque — not expanded; only its address is taken. // boundary
struct propHALO_OBJ_DESC_vtbl;

struct propHALO_OBJ_DESC : propENT_DESC {
    DEFAULT_CTOR<bool> isEntSslClassInited; // 0x34 lazy-init flag for the ssl class link
    unsigned char _pad35[3];        // 0x35
    int   sslEventId_OnChangeInA;   // 0x38
    int   sslEventId_OnChangeInB;   // 0x3C
    int   sslEventId_OnChangeInC;   // 0x40
    int   sslEventId_OnChangeInD;   // 0x44
    int   sslEventId_OnChangeOutA;  // 0x48
    int   sslEventId_OnChangeOutB;  // 0x4C
    int   sslEventId_OnChangeOutC;  // 0x50
    int   sslEventId_OnChangeOutD;  // 0x54
    apSTATE_T<unsigned long> objUse; // 0x58
    // 0x5C configured termination-time policy: a value below M3D_EPSILON_5 (~0) means
    // "terminate immediately" — checked by propHALO_OBJ::OwnerTerm.
    float timeTerm;

    static propHALO_OBJ_DESC_vtbl vftable; // `propHALO_OBJ_DESC::`vftable'' — boundary, not decompiled

    // DB-verified static (types_members / names: propHALO_OBJ_DESC::TYPE_ID @ 0x84287674) — the
    // class-wide type-id bitmask copied out by TypeID() below.
    static dsTYPE_ID TYPE_ID;

    // 0x823EB270 — base-construct, install this class's vtable, and default the lazily-init'd
    // fields (isEntSslClassInited=false, objUse=0, timeTerm=0).
    propHALO_OBJ_DESC();

    // 0x823EB3C8 (virtual) — reset the vtable back to propBASE_DESC's own (standard destructor
    // vptr unwind), then tear down sslScripts and sslClass, then the dscDESC base.
    ~propHALO_OBJ_DESC();

    // 0x823EBB88 (virtual) — compiler-generated deleting-destructor entry point (vector-deleting
    // variant; DB shows only the (this, flags) shape, no array-count parameter).
    propHALO_OBJ_DESC *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823E4570 — memberwise copy-assign (propBASE_DESC base, then every field below timeTerm).
    propHALO_OBJ_DESC &operator=(const propHALO_OBJ_DESC &that);

    // 0x823EB2C8 (virtual, const) — copy this class's static TYPE_ID bitmask into `result` and
    // return it. sret; the disassembly never dereferences `this` (only the static TYPE_ID and the
    // caller-supplied result pointer), so the DB's (this, result) parameter order is register
    // truth here, not a decompiler sret swap.
    dsTYPE_ID TypeID() const;

    // 0x823EB370 (virtual) — mark the lazy ssl-class-init flag done.
    void MarkSslClassInited();

    // 0x823EB380 (virtual) — mark the ssl-class-init flag done when `typeId` matches this class's
    // own TYPE_ID (hierarchy-wide SSL init propagation helper).
    void SetSslInitFlagsForHier(const dsTYPE_ID &typeId);

    // 0x823EB808 (virtual) — after the propBASE_DESC base init, lazily register this class's eight
    // OnChange event declarations (In/Out x A-D) on the shared sslClass ref, short-circuiting on
    // the first registration error. Returns whether initialisation is (now) complete.
    int InitSslClass(const dsTSTRING<char> &hintErr);

    // 0x823EBBF8 (virtual) — after the propBASE_DESC base parse, read "timeTerm" and the "use"
    // string array, OR-ing a distinct bit into objUse.state per recognised name (falling back to
    // bit 0x1 for any unrecognised/absent value).
    void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);
};
