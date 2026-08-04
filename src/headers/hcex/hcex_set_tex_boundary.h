#pragma once
/* Boundary declarations for hcex_set_tex. txmTEXTURE is the real ws-engine (txm*) class — the full
 * DB-verified definition lives in ws/txm/txmTEXTURE.h and is used directly (texSize @0x70), plus
 * its virtual Set method (vtable slot 19 / offset 0x4C, DB-verified txmTEXTURE_vtbl / txmD3D_TEX_vtbl:
 * `int (__fastcall *)(txmTEXTURE *this, pctPICTURE *, int)`), declared here in the free-function
 * boundary form. */

#include "../pct_boundary.h"        /* pctPICTURE */
#include "../ws/txm/txmTEXTURE.h"   /* txmTEXTURE (full DB layout) */

/* txmTEXTURE::Set (virtual, vtable slot 19 / offset 0x4C) — upload `pict` into this texture. */
extern int txmTEXTURE__Set(txmTEXTURE *self, pctPICTURE *pict, int flag);
