#pragma once
#include "../ds/WEAK_PTR_BASE.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTYPE_ID.h"
#include "../ssl/sslOBJ_REF.h"
#include "../ds/dsSTRID.h"
// NOTE: iaIACTOR is used here only by pointer (pOwner). Including ../ia/iaIACTOR.h creates an
// include cycle (iaIACTOR embeds propCONTAINER by value, and propCONTAINER derives from propBASE,
// so propBASE must be fully defined before iaIACTOR) — forward-declare instead of #include.
struct iaIACTOR;
// ws-engine prop subsystem: root of the property/component object hierarchy every prop-family
// class (propENT, propINST_CONSTRUCTOR, ...) derives from.
// DB-verified layout (types_members propBASE):
//   __vftable@0, ds::WEAK_PTR_BASE<propBASE>@4, pPropOwner@8 (propCONTAINER*), pOwner@12
//   (iaIACTOR*), spDesc@16 (dsSMART_PTR<propBASE_DESC,...>), sslObject@20 (sslOBJ_REF),
//   typeId@24 (dsTYPE_ID, 16B) — size 40.

struct propCONTAINER;  // ../prop/propCONTAINER.h — owning container            boundary (pointer only)
struct propBASE_DESC;  // description block owned via dsSMART_PTR               boundary (pointer only)
struct propBASE;

// Virtual dispatch table (types_members propBASE_vtbl has 30+ entries covering the full
// prop lifecycle: InitProperty/Show/Hide/ProcessINIT/ProcessFRAME/... ). Not one of those
// virtuals is called by the functions reversed in this batch, so the table is left opaque
// (forward-declared only) rather than fully modeled here — a same-subsystem accretion point
// for a future batch that actually calls through it.
struct propBASE_vtbl;

typedef struct propBASE {
    propBASE_vtbl               *__vftable;   // 0x00
    ds::WEAK_PTR_BASE<propBASE>  weakSelf;     // 0x04
    propCONTAINER                *pPropOwner;  // 0x08 owning container
    iaIACTOR                     *pOwner;       // 0x0C owning actor (base ptr; in practice an entENTITY — see ../ent/entENTITY.h)
    dsSMART_PTR<propBASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x10
    sslOBJ_REF                   sslObject;    // 0x14
    dsTYPE_ID                    typeId;       // 0x18

    // 0x826C3658 — default construct. Body external to this batch. // boundary
    propBASE();
    // 0x823EB518 (virtual) — destruct. Body external to this batch. // boundary
    virtual ~propBASE();

    // Per-entry probe called by iaIACTOR::GetProperty<P>() (see ../ia/iaIACTOR.h) while scanning
    // propContainer.properties/components: returns this propBASE reinterpreted as P* when it (or
    // its own nested property collection) matches P's runtime type, else nullptr. Not reversed in
    // this batch (out of scope — only the iaIACTOR-level callers were reversed). boundary.
    template<class P> P *GetProperty();

    // Referenced by dsVECTOR<propBASE*,8>::Find<dsNAMEID_CMP<propBASE,0,0>,char const*>
    // (0x826C3728) as `pData[i]->GetName()`; returns the prop's interned name id by value
    // (sret). Body external to this batch. // boundary
    dsSTRID GetName() const;
} propBASE;
