#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "dscDESC.h"
// boundary — dsc (descriptor system) brand/type registry object. `Init` is reversed in full;
// the surrounding members are reproduced faithfully (DB-verified offsets) but most of their
// own methods (fnCopyDesc/fnCreateDesc bodies, dscBRAND_vtbl slots beyond dtr/InitFromParent)
// are external to this batch.
// DB-verified layout (types_members dscBRAND):
//   __vftable@0, fnCopyDesc@4, fnCreateDesc@8, isCodeRegistered@12, name@16 (dsTSTRING<char>),
//   isInited@20, pParent@24, spDesc@28, brandStateLoad@32 — size 36.

struct dscDESC_FAMILY;

// DB-verified (types_members dscBRAND_vtbl): dtr_dscBRAND@0, InitFromParent@4 — size 8
// (only these two slots observed so far; accrete further slots here as they're reversed).
typedef struct dscBRAND_vtbl {
    void (*dtr_dscBRAND)(dscBRAND *self, int deleteFlag);                    // 0x00 (deleting dtr; takes a free flag) — deleting dtor: vftable+0x00 holds ??_EdscBRAND@@UAAPAXI@Z
    void (*InitFromParent)(dscBRAND *self, dscBRAND *parent); // 0x04
} dscBRAND_vtbl;

// Per-brand load-lifetime classification (DB-verified: types_enum_values dscBRAND::STATE_LOAD;
// hoisted to namespace scope for the layout verifier).
enum STATE_LOAD {
    BSL_UNKNOWN   = 0,   // not yet classified
    BSL_LEVEL_DEP = 1,
    BSL_PERMANENT = 2,
};

typedef struct dscBRAND {
    dscBRAND_vtbl   *__vftable;         // 0x00
    void (*fnCopyDesc)(dscDESC *dst, dscDESC *src);      // 0x04
    dscDESC *(*fnCreateDesc)(const char *name, int n);   // 0x08
    bool             isCodeRegistered;  // 0x0C
    unsigned char    _pad0D[3];         // 0x0D
    dsTSTRING<char>  name;              // 0x10 brand name
    bool             isInited;          // 0x14
    unsigned char    _pad15[3];         // 0x15
    dscBRAND        *pParent;           // 0x18
    // 0x1C dsSMART_PTR<dscDESC,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>> — single owning pointer
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc;
    STATE_LOAD       brandStateLoad;    // 0x20

    // 0x82711A70 — one-time lazy init: recursively init the parent brand first and copy-inherit
    // its descriptor via fnCopyDesc, link this brand's owned descriptor back to `this`, look up
    // this brand's config-file section by name in the owning family's pending-section map
    // (asserts the lookup must succeed), and run it through the descriptor's ProcessPS. No-op
    // if already inited (isInited).
    void Init(dscDESC_FAMILY *pFamily);
} dscBRAND;
