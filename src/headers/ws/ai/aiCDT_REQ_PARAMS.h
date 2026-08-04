#pragma once
#include "../m3d/m3dV.h"
#include "CONST_ARRAY_m3dV.h"
#include "../ds/dsVECTOR.h"

// ws-engine ai08 (a8_def_cdt_sys) — parameter blocks passed to aiDEF_CDT_SYS::Post*Request. All
// DB-verified layouts (types_members). aiBRAIN is referenced by pointer only.
struct aiBRAIN;
struct entENTITY; // ent subsystem — game entity boundary (ptr)

// aiREFINE_TRAJ_NEAR / aiREFINE_TRAJ_FAR — trajectory-refine sub-blocks (size 56 each), DB-verified
// layouts in their own headers (aiREFINE_TRAJ_COMMON base over cdtREFINE).
#include "aiREFINE_TRAJ_NEAR.h"
#include "aiREFINE_TRAJ_FAR.h"

// DB-verified layout (types_members aiVIS_REQ_PARAMS), size 0x40.
struct aiVIS_REQ_PARAMS {
    aiBRAIN *sender;       // 0x00
    m3dV     posEye;       // 0x04
    m3dV     posHeightMin; // 0x10
    m3dV     posCheck;     // 0x1C
    m3dV     posHeightMax; // 0x28
    m3dV     dirUp;        // 0x34

    // ?aiVIS_REQ_PARAMS::aiVIS_REQ_PARAMS(void) — zero-init. boundary.
    aiVIS_REQ_PARAMS();
    // ?FillParams@aiVIS_REQ_PARAMS@@QAAXPAVaiBRAIN@@ABUm3dV@@1111@Z — populate every field from the
    // supplied brain + five sample points. boundary.
    void FillParams(aiBRAIN *snd, const m3dV &eye, const m3dV &heightMin, const m3dV &check,
                    const m3dV &heightMax, const m3dV &up);
};

// DB-verified layout (types_members aiREACH_REQ_PARAMS), size 0x1C.
struct aiREACH_REQ_PARAMS {
    aiBRAIN *sender;  // 0x00
    m3dV     posFrom; // 0x04
    m3dV     posTo;   // 0x10
};

// DB-verified layout (types_members aiFE_REQ_PARAMS), size 0x18.
struct aiFE_REQ_PARAMS {
    // One candidate entity plus its visibility geometry. DB-verified layout, size 68.
    struct enCANDIDATE {
        entENTITY       *ent;    // 0x00
        aiVIS_REQ_PARAMS params; // 0x04
    };

    aiBRAIN                    *pSender;    // 0x00
    dsVECTOR<enCANDIDATE, 8>    candidates; // 0x04

    // ?aiFE_REQ_PARAMS::aiFE_REQ_PARAMS(void) — empty request. boundary.
    aiFE_REQ_PARAMS();
};

// DB-verified layout (types_members aiTRAJ_REQ_PARAMS), size 0xF8.
struct aiTRAJ_REQ_PARAMS {
    aiBRAIN               *pSender;         // 0x00
    int                    sgidx;           // 0x04 sub-group index
    CONST_ARRAY_m3dV<10>   points;          // 0x08
    int                    countNearChecks; // 0x84
    aiREFINE_TRAJ_NEAR     refineNear;      // 0x88
    aiREFINE_TRAJ_FAR      refineFar;       // 0xC0
};

// DB-verified layout (types_members aiDNS_PNT_REQ_PARAMS), size 0x1C.
struct aiDNS_PNT_REQ_PARAMS {
    aiBRAIN *pSender;   // 0x00
    m3dV     posCheck;  // 0x04
    m3dV     posCenter; // 0x10
};
