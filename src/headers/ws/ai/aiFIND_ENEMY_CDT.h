#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/WEAK_PTR.h"
#include "../m3d/m3dV.h"
// aiVIS_REQ_PARAMS / aiFE_REQ_PARAMS (+ nested enCANDIDATE) are defined canonically here:
#include "aiCDT_REQ_PARAMS.h"

// ws-engine ai08 — the "find enemy" request/response records and the default collision-detection
// (visibility) request system (aiDEF_CDT_SYS) that services them. DB-verified layouts
// (types_members aiVIS_REQ_PARAMS / aiFE_REQ_PARAMS / aiFE_REQ_PARAMS::enCANDIDATE / aiFE_REQ_RES).
// aiDEF_CDT_SYS's internal maps are the next frontier; it is modelled as a size-accurate opaque
// block carrying only the query surface aiFILT_ENEMY reaches. boundary.

struct aiBRAIN;   // ai08 — per-unit AI brain   boundary (ptr)
struct entENTITY; // ent subsystem — game entity boundary (ptr)

// aiFE_REQ_RES — a find-enemy result: the visible candidate (weak ref). DB-verified layout, size 4.
struct aiFE_REQ_RES {
    ds::WEAK_PTR<entENTITY> candVisible; // 0x00

    // ?aiFE_REQ_RES::aiFE_REQ_RES(void) / ~aiFE_REQ_RES(void) — null/release the weak ref. boundary.
    aiFE_REQ_RES();
    ~aiFE_REQ_RES();
};

// aiDEF_CDT_SYS — the default visibility-request system (aiPLANNER::defCDT). Canonical full
// definition (vtbl + srcReqs/dstReqs maps + dbgRays) lives in aiDEF_CDT_SYS.h.
#include "aiDEF_CDT_SYS.h"
