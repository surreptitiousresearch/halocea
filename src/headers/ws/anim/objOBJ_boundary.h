#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsARRAY.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dVTX.h"
#include "../m3d/m3dBOX.h"
#include "../m3d/m3dOBB.h"
#include "../mtl/mtlTBL.h"
#include "../ps/psSECTION.h"
// anim-local view of the obj subsystem, for the anim validation/LOD/render-list methods.
//
// This header used to carry its OWN bodies for objOBJ, objGEOM_SHARED, objGEOM_UNSHARED, objSPLIT,
// objSPLIT_ENTRY and objBONES_INFO, guarded by hand-rolled `#ifndef X_DEFINED` macros — six types
// with two file-scope definitions each, where which body a TU saw depended on include order. The
// stated reason ("the real ws/obj/objOBJ.h re-includes a conflicting legacy-Blam animINST.h") no
// longer holds: ws/anim/animINST_boundary.h now forwards to the single canonical src/headers/
// animINST.h, which is exactly what ws/obj/objOBJ.h includes. So this file just pulls the canonical
// obj graph.
//
// Adjudication of the six deleted bodies against the DB (types / types_members), all offsets
// identical on both sides; the differences were padding SPELLING plus these:
//   * objGEOM_UNSHARED — the deleted body's explicit `_pad47[1]` tail member was the correct read
//     (DB size 72, last member transpRendPriority@0x46); ws/obj/objGEOM_UNSHARED.h said "size 71"
//     and has been corrected there.
//   * objSPLIT::GetEntry — the deleted body claimed 0x8265F2A8. There is no function at that
//     address in the DB. The surviving ws/obj/objSPLIT.h address 0x82681828 is
//     ?GetEntry@objSPLIT@@QAAPAVobjSPLIT_ENTRY@@H@Z. Nothing merged.
//   * objOBJ::GetVert (0x82BCE098) and objOBJ::GetOBB's address (0x82A719D8) existed only here;
//     both are DB-confirmed and have been merged into ws/obj/objOBJ.h.
#include "../obj/objOBJ.h"     // objOBJ (canonical) -> objGEOM_UNSHARED.h -> objGEOM_SHARED.h
#include "../obj/objSPLIT.h"   // objSPLIT / objSPLIT_ENTRY / objBONES_INFO (canonical)

struct objMODIFIER;      // obj subsystem modifier (pointer only)        boundary
struct objSPU_CONFIG;    // boundary — pointer only
struct objFACE;          // boundary — pointer only
struct m4dV;             // ../m3d/m4dV.h  — pointer only
struct rendORD;          // rend subsystem render-order data (pointer only) boundary
