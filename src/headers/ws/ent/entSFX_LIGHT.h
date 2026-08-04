#pragma once
#include "entSFX.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsTYPE_ID.h"
#include "../rend/rendLIGHT.h"
// ws-engine ent subsystem: light special-effect attachment. DB-verified layout
// (types_members entSFX_LIGHT) — size 52 (0x34):
//   entSFX (base)@0 (36), pNext@0x24, pPrev@0x28 (intrusive list),
//   lgtState@0x2C (apSTATE_T<unsigned long>), pLight@0x30 (dsSMART_PTR<rendLIGHT,...>).

// Descriptor for entSFX_LIGHT; only its runtime-type static is needed here.
struct entSFX_LIGHT_DESC {
    static dsTYPE_ID TYPE_ID; // ?TYPE_ID@entSFX_LIGHT_DESC@@2VdsTYPE_ID@@A @ 0x8427CAA8
};

typedef struct entSFX_LIGHT : entSFX {
    entSFX_LIGHT             *pNext;    // 0x24
    entSFX_LIGHT             *pPrev;    // 0x28
    apSTATE_T<unsigned long>  lgtState; // 0x2C
    dsSMART_PTR<rendLIGHT, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > pLight; // 0x30

    // 0x826F2A90 — set the underlying runtime light's radius scale (lgtLIGHT::scale @ 0xB8), when
    // a light record is bound. No-op if pLight is null.
    void SetRadScale(float scale);
} entSFX_LIGHT;
