#pragma once
#include "../m3d/m3dV.h"
// ws-engine fsm: the parameter block delivered to a brain when a collision-detection query completes.
// DB-verified layout (types_members fsmPARAM_CDT): fsmEVENT_PARAM_BASE@0 + fsmCB_PARAM_BASE@1 (base),
// trIn@4, trOut@0x10, cdtAnim@0x1C, cdtGeom@0x20.

struct cdtINFO; // cdt/cdtINFO.h — collision-query result record (fwd, ptr)

struct fsmPARAM_CDT {
    unsigned char _base[4]; // 0x00 fsmEVENT_PARAM_BASE + fsmCB_PARAM_BASE (opaque base)
    m3dV     trIn;          // 0x04 nav-target-in position
    m3dV     trOut;         // 0x10 nav-target-out position
    cdtINFO *cdtAnim;       // 0x1C animation-driven collision result
    cdtINFO *cdtGeom;       // 0x20 geometry-driven collision result
};
