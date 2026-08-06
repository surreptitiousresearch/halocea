#pragma once
/* Boundary declarations for hcex_mark_texture_stage. txmTEXTURE is the real ws-engine (txm*) class —
 * the full DB-verified definition lives in ws/txm/txmTEXTURE.h and is used directly (name @0x04,
 * sizeZ @0x0C). GetSizeX/GetSizeY are non-virtual (QBA) accessors, called directly (not through the
 * vtable), declared here in the free-function boundary form. */

#include "../ws/txm/txmTEXTURE.h"   /* txmTEXTURE (full DB layout), dsTSTRING<char> */

extern int txmTEXTURE__GetSizeX(const txmTEXTURE *self);
extern int txmTEXTURE__GetSizeY(const txmTEXTURE *self);

extern "C" void hcex_pix_marker(const char *fmt, ...);
