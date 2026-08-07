#pragma once
#include "../wb/dbgVAR_boundary.h"
// ws-engine dbg: a trigger-typed debug variable -- a named leaf of the dbgVAR hierarchy shared
// with wb (watch/debug window), sibling to dbgVAR_STRING/dbgVAR_BOOL/dbgVAR_INT/dbgVAR_FLOAT
// (see dbgVAR_STRING.h). Canonical home for the type; it previously had a body in BOTH
// hcex/hcex_set_cam_boundary.h and ws/strm/strm_boundary.h (identical), which is an
// `error: redefinition` in the header_layout probe TU.
//
// DB-verified layout (types_members dbgVAR_TRIGGER): a single ANONYMOUS base-class member of
// type dbgVAR_SIMPLE<bool,5> at offset 0, size 16 -- `types` reports size 16 for the whole type.
// Modelled as a NAMED member `base` rather than as inheritance (the spelling dbgVAR_STRING.h
// uses) because both consumers read the payload as `.base.value`
// (hcex_set_cam.cpp:20 dbg_hcex_DumpModels.base.value, strmRES_TEX_MANAGER__UpdateResource2.cpp:84
// d2.base.value); layout is identical either way.

typedef struct dbgVAR_TRIGGER
{
    dbgVAR_SIMPLE<bool,5> base;  /* 0x00 -- bool payload at base offset 0x0C (dbgVAR_IMPL layer;
                                    the template home is ws/wb/dbgVAR_boundary.h) */
} dbgVAR_TRIGGER;
