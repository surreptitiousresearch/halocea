#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTYPE_ID.h"
#include "../ap/apSTATE_T.h"
#include "../dsc/dscDESC.h"
// ws-engine ent subsystem: base class for entity special-effect attachments (light, colorizer,
// particle, trail, ...). DB-verified layout (types_members entSFX) — size 36 (0x24):
//   __vftable@0, dbgID@4, name@8 (dsSTRID), state@0xC (apSTATE_T<unsigned char>),
//   spDesc@0x10 (dsSMART_PTR<dscDESC,...>), pBrand@0x14 (dscBRAND*), timeCur@0x18,
//   timeBeforeStart@0x1C, pObjParent@0x20 (objOBJ*).

struct entSFX_vtbl;   // opaque — full sfx virtual table not modeled here      boundary
struct dscBRAND;      // dsc subsystem (pointer only)                          boundary
struct objOBJ;        // obj subsystem (pointer only)                          boundary
struct dscTYPE_INFO;  // dsc runtime type-info; begins with a dsTYPE_ID        boundary

typedef struct entSFX {
    entSFX_vtbl              *__vftable;        // 0x00
    int                      dbgID;            // 0x04
    dsSTRID                  name;             // 0x08
    apSTATE_T<unsigned char> state;            // 0x0C
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x10
    dscBRAND                *pBrand;           // 0x14
    float                    timeCur;          // 0x18
    float                    timeBeforeStart;  // 0x1C
    objOBJ                  *pObjParent;       // 0x20

    // 0x823D1E08 (IsA<entSFX_LIGHT>) / 0x823EAD48 (IsA<entSFX_COLOR>) — runtime-type predicate:
    // query the descriptor's current type-info (virtual) and test whether it derives from P's
    // descriptor TYPE_ID. Target of the hcex flattened externs entSFX_IsA_LIGHT / entSFX_IsA_COLOR.
    template<class P> int IsA() const;

    // Per-camera sfx visibility, invoked by entENTITY::ShowToCamera/HideFromCamera for each live
    // instance. Virtual; body is a boundary. idCamera selects the camera slot.
    virtual void implShow(int idCamera);
    virtual void implHide(int idCamera);
} entSFX;
