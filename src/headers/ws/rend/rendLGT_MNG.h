#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "rendLIGHT.h"
// ws-engine rend: the global light manager (singleton `rendLgtMng`). DB-verified layout
// (types_members rendLGT_MNG) — size 16576. Per the project's boundary conventions only the
// members the light-set static update reads are typed (the static-light vector, the big-light
// count, and the collision-light acceleration pointer); everything else is padding at its exact
// DB offset/size so sizeof() and those offsets stay correct.

struct cdtCOLL_LIGHT; // ../cdt/cdtCOLL_LIGHT.h — pointer only here  boundary

// The per-slot smart pointer stored in the static/dynamic light vectors.
typedef dsSMART_PTR<rendLIGHT, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > rendLIGHT_SP;

typedef struct rendLGT_MNG {
    void                   *__vftable;      // 0x0000
    dsVECTOR<rendLIGHT_SP, 8> staticLights; // 0x0004 (20)
    unsigned char           _pad0018[15700];// 0x0018 .. 0x3D6C dynamic lights/active buffers/etc  boundary
    int                     lastBigLightNmb;// 0x3D6C (15724)
    cdtCOLL_LIGHT          *pCollLight;     // 0x3D70 (15728)
    unsigned char           _pad3D74[844];  // 0x3D74 .. 0x40C0 (16576) remaining manager state  boundary

    // 0x... — fetch the ref-counted static light at `idx`. Body lives in the rend subsystem —
    // boundary.
    const rendLIGHT_SP *GetStaticLight(int idx); // boundary

    // Body lives in the rend subsystem — boundary (called from hcex_change_render_mode).
    void ClearActiveLights();                        // boundary
    void PushLightsForRenderingPreviousFrame();      // boundary
} rendLGT_MNG;

extern rendLGT_MNG *rendLgtMng; // global light-manager singleton  boundary
