#pragma once
#include "rayCOLLECTOR.h"
#include "RAY_RELATED_ERROR.h"
// ws-engine ho: debug-info base subobject of hoEDGE. DB-verified layout (types_members
// hoEDGE_DBG_INFO): __vftable@0, rayCollector@4 (rayCOLLECTOR, 60), error@64
// (RAY_RELATED_ERROR, 84), id_edge_dbg@148 — size 152 (0x98). Explicit __vftable member +
// no virtual methods per project vptr convention.

struct hoEDGE_DBG_INFO_vtbl;

struct hoEDGE_DBG_INFO {
    hoEDGE_DBG_INFO_vtbl *__vftable;   // 0x00
    rayCOLLECTOR          rayCollector; // 0x04
    RAY_RELATED_ERROR     error;        // 0x40
    int                   id_edge_dbg;  // 0x94
};
